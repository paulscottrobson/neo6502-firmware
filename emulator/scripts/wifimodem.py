# Experimental Wi-Fi modem emulator for testing TCP serial connection

import socket
import threading
import re
import select
import binascii

class WifiModemEmulator:
    def __init__(self, host='localhost', port=25232):
        self.host = host
        self.port = port
        self.server_socket = None
        self.running = False
        self.wifi_mode = 1  # Station mode by default
        self.wifi_connected = False
        self.ip_address = None
        self.active_connection = None
        self.echo_enabled = True  # Echo is enabled by default
        self.command_buffer = b''
        self.cipsend_mode = False
        self.cipsend_length = 0
        self.client_socket = None

    def start(self):
        self.running = True
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(1)

        print(f"Wi-Fi Modem Emulator listening on {self.host}:{self.port}")
        while self.running:
            client_socket, addr = self.server_socket.accept()
            print(f"Connected by {addr}")
            client_thread = threading.Thread(target=self.handle_client, args=(client_socket,))
            client_thread.start()

    def handle_client(self, client_socket):
        self.client_socket = client_socket
        self.command_buffer = b''
        while self.running:
            try:
                data = client_socket.recv(1024)
                if not data:
                    break
                
                print("\n--- Received data from client ---")
                print("Hexdump:")
                print(self.hexdump(data))
                print("--- End of received data ---\n")
                
                self.command_buffer += data
                
                while self.command_buffer:
                    if self.cipsend_mode:
                        if len(self.command_buffer) >= self.cipsend_length:
                            send_data = self.command_buffer[:self.cipsend_length]
                            self.handle_cipsend_data(client_socket, send_data)
                            self.command_buffer = self.command_buffer[self.cipsend_length:]
                        else:
                            break  # Wait for more data
                    else:
                        if b'\r\n' in self.command_buffer:
                            command, self.command_buffer = self.command_buffer.split(b'\r\n', 1)
                            command = command.strip().decode('utf-8', errors='ignore')
                            if command:
                                print(f"Received command: {command}")
                                response = self.process_command(command)
                                
                                print("\n--- Sending data to client ---")
                                print("Hexdump:")
                                print(self.hexdump(response.encode('utf-8')))
                                print("--- End of sent data ---\n")
                                
                                client_socket.sendall(response.encode('utf-8'))
                        else:
                            break  # Wait for more data
                        
            except Exception as e:
                print(f"Error handling client: {e}")
                break
        self.client_socket = None
        client_socket.close()

    def process_command(self, command):
        # Convert command to uppercase for case-insensitive matching
        upper_command = command.upper()
        
        if self.echo_enabled:
            echo_response = command + '\r\n'
        else:
            echo_response = ''

        if upper_command == "AT":
            return echo_response + "OK\r\n"
        elif upper_command == "AT+RST":
            return echo_response + "OK\r\n"
        elif upper_command.startswith("AT+CWMODE"):
            return echo_response + self.handle_cwmode(upper_command)
        elif upper_command.startswith("AT+CWJAP"):
            return echo_response + self.handle_cwjap(upper_command)
        elif upper_command == "AT+CIFSR":
            return echo_response + self.handle_cifsr()
        elif upper_command.startswith("AT+CIPSTART"):
            return echo_response + self.handle_cipstart(upper_command)
        elif upper_command.startswith("AT+CIPSEND"):
            return echo_response + self.handle_cipsend(upper_command)
        elif upper_command == "AT+CIPCLOSE":
            return echo_response + self.handle_cipclose()
        elif upper_command in ["ATE", "ATE1"]:
            return self.handle_ate(True)
        elif upper_command == "ATE0":
            return self.handle_ate(False)
        elif upper_command == "AT+CIPSTATUS":
            return echo_response + self.handle_cipstatus()
        else:
            print(f"DEBUG: Unrecognized command: {command}")
            return echo_response + "ERROR\r\n"

    def handle_cwmode(self, command):
        mode = re.search(r'AT\+CWMODE=(\d)', command)
        if mode:
            self.wifi_mode = int(mode.group(1))
            return "OK\r\n"
        return "ERROR\r\n"

    def handle_cwjap(self, command):
        self.wifi_connected = True
        self.ip_address = "192.168.1.100"
        return "WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"

    def handle_cifsr(self):
        response = ""
        if self.ip_address:
            response += f"+CIFSR:STAIP,\"{self.ip_address}\"\r\n"
        if self.wifi_mode in [2, 3]:  # AP or AP+STA mode
            response += f"+CIFSR:APIP,\"192.168.4.1\"\r\n"
        return response + "\r\nOK\r\n" if response else "ERROR\r\n"

    def handle_cipstart(self, command):
        match = re.search(r'AT\+CIPSTART="(\w+)","(.+)",(\d+)', command)
        if match:
            protocol, host, port = match.groups()
            try:
                self.active_connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.active_connection.connect((host, int(port)))
                # Start a new thread to read from the active connection
                threading.Thread(target=self.read_from_active_connection, daemon=True).start()
                return "CONNECT\r\n\r\nOK\r\n"
            except Exception as e:
                print(f"Failed to connect: {e}")
                self.active_connection = None
                return "ERROR\r\n"
        return "ERROR\r\n"

    def handle_cipsend(self, command):
        match = re.search(r'AT\+CIPSEND=(\d+)', command)
        if match and self.active_connection:
            self.cipsend_length = int(match.group(1))
            self.cipsend_mode = True
            return "OK\r\n> "
        return "ERROR\r\n"

    def handle_cipsend_data(self, client_socket, data):
        print("\n--- Sending data to server ---")
        print("Hexdump:")
        print(self.hexdump(data))
        print("--- End of sent data ---\n")
        
        try:
            self.active_connection.sendall(data)
            response = "SEND OK\r\n"
            
            print("\n--- Sending data to client ---")
            print("Hexdump:")
            print(self.hexdump(response.encode('utf-8')))
            print("--- End of sent data ---\n")
            
            client_socket.sendall(response.encode('utf-8'))
        except Exception as e:
            print(f"Failed to send data: {e}")
            error_response = "ERROR\r\n"
            
            print("\n--- Sending error to client ---")
            print("Hexdump:")
            print(self.hexdump(error_response.encode('utf-8')))
            print("--- End of sent data ---\n")
            
            client_socket.sendall(error_response.encode('utf-8'))
        self.cipsend_mode = False

    def read_from_active_connection(self):
        buffer_size = 8192  # 8K buffer
        while self.active_connection and self.running:
            try:
                ready, _, _ = select.select([self.active_connection], [], [], 1)
                if ready:
                    data = self.active_connection.recv(buffer_size)
                    if data:
                        print("\n--- Received data from server ---")
                        print("Hexdump:")
                        print(self.hexdump(data))
                        print("--- End of received data ---\n")

                        if self.client_socket:
                            # Format the +IPD message
                            ipd_msg = f"+IPD,{len(data)}:"
                            response = ipd_msg.encode('ascii') + data

                            print("\n--- Sending data to client ---")
                            print("Hexdump:")
                            print(self.hexdump(response))
                            print("--- End of sent data ---\n")

                            self.client_socket.sendall(response)
                    else:
                        # Connection closed by the server
                        print("Server closed the connection")
                        self.handle_cipclose()
                        break
            except Exception as e:
                print(f"Error reading from active connection: {e}")
                self.handle_cipclose()
                break

    def handle_cipclose(self):
        if self.active_connection:
            self.active_connection.close()
            self.active_connection = None
        if self.client_socket:
            response = "CLOSED\r\n"
            
            print("\n--- Sending data to client ---")
            print("Hexdump:")
            print(self.hexdump(response.encode('utf-8')))
            print("--- End of sent data ---\n")
            
            self.client_socket.sendall(response.encode('utf-8'))
        self.cipsend_mode = False
        self.cipsend_length = 0
        return "OK\r\n"

    def handle_ate(self, enable):
        self.echo_enabled = enable
        return "OK\r\n"

    def handle_cipstatus(self):
        response = "STATUS:2\r\n"  # Assuming 2 means "Got IP"
        if self.active_connection:
            response += "+CIPSTATUS:0,\"TCP\",\"" + self.active_connection.getpeername()[0] + "\"," + str(self.active_connection.getpeername()[1]) + ",0\r\n"
        return response + "\r\nOK\r\n"

    def hexdump(self, data):
        def grouped(iterable, n):
            return zip(*[iter(iterable)]*n)

        result = []
        for i, chunk in enumerate(grouped(binascii.hexlify(data).decode(), 32)):
            hex_line = ' '.join(''.join(pair) for pair in grouped(chunk, 2))
            ascii_line = ''.join(chr(b) if 32 <= b < 127 else '.' for b in data[i*16:(i+1)*16])
            result.append(f"{i*16:04x}: {hex_line:<48}  {ascii_line}")
        return '\n'.join(result)

    def stop(self):
        self.running = False
        if self.server_socket:
            self.server_socket.close()
        if self.active_connection:
            self.active_connection.close()
        if self.client_socket:
            self.client_socket.close()

if __name__ == "__main__":
    emulator = WifiModemEmulator()
    try:
        emulator.start()
    except KeyboardInterrupt:
        print("Stopping the emulator...")
    finally:
        emulator.stop()
