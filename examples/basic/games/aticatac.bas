     �     �     �     �     �    INITIALISE.GAME�     ENERG�    DRAW.ROOM�     TMOV�    MOVE.PLAYER�     TMISSIL�     TMISSILEEN�    MOVE.MISSILE�     TCHEC�    CHECK.OPEN.CLOSE.WAKE�    MON.TIMER�     CURR.MONSTE�    MOVE.MONSTER�     LIVE�     GAME.DAT�
     SIZ�     MAX.DEPT�     LEVE�     X.STAR�     Y.STAR�    DOORS�    DOOR.OPEN�    DOOR.TIMER�    MON.GRAPHIC�    MON.XI�    MON.YI�    MON.SPEED�    MON.IQ�    MON.ALIVE�     SCOR�     X.ROO�     Y.ROO�     X.PLAYE�     Y.PLAYE�     XI.PLAYE�     YI.PLAYE�     X.FLI�    KEY.COUNT�     �
     FIR�    HIT.WALL�     FLOOR.CENTR�    ANIMATE.UP�    ANIMATE.DOWN�    COLLECT.OBJECT�     X.MISSIL�     Y.MISSIL�     XI.MISSIL�     YI.MISSIL�    REMOVE.OBJECT�     CONTENT�    DRAW.ENERGY�    DRAW.KEYS�     �     MOVE�    CHECK.UNLOCK�    ADD.SCORE�     �     �    RANDOM.SEED.ROOM�    DRAW.ROOM.OUTLINE�    READ.MAP�    MAKE.DOOR�    RANDOM.NEXT�     X.CONTENT�
     SEE�     Y.CONTENT�     CONTNT�    DRAW.WALL.ELEMENT�    DRAW.SCORE�    DRAW.DOOR�    CREATE.MONSTER�     �    REDIRECT.MONSTER�    HIT.PLAYER�    HIT.MONSTER�    KILL.MONSTER�	     IM�     OFFSE�	     RO�     �     �     �
     TYP�     C�    LINE.DRAW�     X�     Y�     C�    RANDOM.SEED�     �
     BIT�                                                                                                                                                                              d ��n ̀*** Atic Atac ***�x ��� � %��� ��	�  ;0Ad�� � G��� ��� �� W�Cο� b��� �� t�Bο� �,@�� ���� �� ��Yο� ���� � � ��,@�� � ����  �0� � A�(C�	� � ;-@�� 0!A�	� �0@������̀initialise game�"��,� %��6���aticatac.gfx�@0�`@�����aticatac.dat��4J̀-initialise level and dimensions, put at start�T0L�'0E�^60@�A0D�N0D�/h̀(door records, created when room entered.�#r��[C��̀Values 0-7 see map.py�7|��gC��̀)true if door is open, false if shut/wall.�,���wC��̀timer for door opening/closing��̀monster records�*����D��̀monster graphic (when alive)�7����D���D��̀$movement use sprite.x functions etc.�J��� �D���D��̀7event variables for movement. timer -1 means not inuse.�l����D��̀^intelligence ; probability out of 100 of redirection / move. If this is 100 then invulnerable.�<����D��̀.flag indicating alive e.g. not in wakeup mode.��̀initialise player��0C��0@��0A��0N��0B@�0Ax� 0@�/0@�>0@����JC��̀key counts 1-3���Z0A�C�JZ�0@̻������̀Move Player.���&� b��0�a�:a0� �/��
D� )/�N0 � %A��X��>0�A! �$A�b0 �/%A��$l�@��CV ��� �$C�(A���>���v��!B@�+x)�!Ax�+x��k��	��z/@����!B@�-P(�!Ax�-P���z0�CU��606!A������ G������606 A������ G��������������@�B�Jο�������	��a/@�	�� �-@� �0�� BV��0��0��0 "C��0/"C� ���)��0@�*�0��C�!A�"C��0��C�!A�"C�4��	>��@�W�H��R��\��f��p̀Collect object�z���������B�����6��������,�Cl�� ;0�Ad� ; �!�Cl�"J O����%��	���@�F�����J!�CY�0J!�CY� A���7��	���@�H���������̀Move missile���$� ���.�A��CX�����8�0� ��B�0� ��L���!B@�+z��0!��V���!Ax�+z��0!��`���+ ���A��� �0!A�j��t��~���̀Hit Wall������k����G�N�N0���0�B@!xʠ�B@ x����0�Ax!xʠ�Ax x������!B@�,x(�!Ax�-J��G0!A�+B@�G0A���YG(C��%��gG(Cο�0�� G��0D@!�N0��������!Ax�,x(�!B@�-J� G0@�+B@�G0B�
�YG(C��&�gG(Cο�0�� G!A�0Cp!�N0����(�N�� G��2��<��$F̀Check unlocked in direction d�P��
Z�YG��d��[G�(C�/@(gG�0@�n�J[G�(C�+@�xJ[G�(C�0J[G�(C�!A��[G�0D�gG�0��wG�0!A��� � 0 � �" 6""�!��G0@�� !ʂ� !�(�CG� �`���G0A�� ʂ� �(�Cx� �D���G0B�� ʂ� �(�CG� �`���G0C�� !Aʂ� !A�(�Cx� �D�	��lT��	���@�I��������������̀	Draw room���� G���Z�|���
"�������,������6��6�60@���B������A�̳�#@��C�ƋT�ˀL�6 Aˀ ��ˀ,���%J̀First is right, second is down� T��|�6����!A����@�|$C��^��|�6��!A������C�|��'h��|�6��������A�|����B�|$C��(r̀!What's in the middle on the floor�|z0@�̀$D4 pit, $D5 ladder���|(�A@��z0�CT �B����60@�z0�CT���60'!A�z0�CU����;�̀4Anything to pick up $DA-$DC keys 1-3 $EC-$EE energy.��0@����|(�B@�/@�������0��&B@� A@������0��&Ap� Ax!\������0�&F�!��,C�0 �Cl���0 �CZ̳����̀Wall decoration��Z0@�C�����&���(�C`�0@(60@�0�0`̲��(Bο�0!��:�!Z��ʁCP ��(A��B��D��N��X�Z0@�D� �Z�0!A̻�b�z/@��z�B@!P�Ax!P�l�/@��B�����v߀1 Up��A��C�DU�D���9���7���%��	��+@���|0A�����CV��AĂ|!A�"P DB�v���������@��CV���	� �0!A�� W0@� t0@� �0@� �0@��������̀read map�������|�6� � ��|0�    " 6""���� ��*̀remove object�4��>�6� � ��H� � 0    " 6""�R� ʜ �(�A�\��f��'p̀ check open/close in current room�z���� ���
��Z�|���Z0@�C���wZ�+@(��+wZ���|0BV̲gZο|0FP��wZ�0�� |��gZ�0gZ�0@�
��JZ�����@��r�B���������Z0@�C��� �Z�-@(�T�0@��ZZ��	��	��	��$	̀Create monster�.	��
8	�Z|��GB	̀@DD pumpkin, E0 jelly E3 spider thing  E6 spikey thing E9 = ghost�L	�Z�o�Z0�E��V	�Z0@��|�0AP���|�0N�`	�Z0A��|�0@��|�0D�j	�Z0B��|�0C��|�0H�t	�Z0C��|�0J��|�0J�~	�Z0D��|�0Ad��|�0^��	�|�0�C] Z"C �C���	�|�0�B�"B!A��|�0�B�"B!A��	 �|�0�� Ad��	�|�0���	o0A 6#B�_��	�|�0�Cʆ�|�#o����	�| J��CXăB@� A@ʃAx� A@��	���	���	̀Move monster��	��
�	� �|��
 
��|��

�� �|���|���

� � �%
 0��| J� �|�� 0��| J� �|��(
�| J� � �2
�� !B@�+x��|�0!�|��<
�� !Ax�+x��|�0!�|��F
��Ad�-�|ο�v|��P
��@�| J�Lο��|��Z
��A�| J�Lο��|��d
��n
��x
�| J�����$C�(A� �CX���D���
���+ �|���
 �|�0@��|�0���
�| J��|���@��
���
���
���
���
̀redirect a monster��
��
�
�v|���
�|�0�!��| J����
�|�0�!��| J����
����̀monster hit player���
"��|��,��|�-Ad�6 ;0 ;!J!�E��@��J ;0 ;!B�T��^��@��CH�E�!Ac�h�%��
r��|��|������̀missile hit monster����
���|�����|�-Ad�
���|��	� �0���	��lr�����	���@�U��������̀
bump score���
�l|���0� |�&�9��0��:��'D̀ kill monster if not invulnerable�N��
X��|��b��|�-Ad�l�| J���v �|�0!A���|�0�����������@�̀9create doors in given direction using preset random seed.�������G�|���|0|(G��[G�0|�gG�0��wG�0!A���|0D�gG�0����|+D�wG�0�FP� Ad���|+@��JG���̀door.open(d) = true�2̀+print d,doors(d),door.open(d),door.timer(d)����� ̀	draw door�*��
4�JG��>����0�CH̲6+@��0�C@�	H�G/D�R�gG�0���0� �[G�(C��\��f�!G�@���D��p��z���̀draw offset element������!G���������� � ���� 0B@ �!P� 0Ax!A@���0@���G0B��0B� 0DP! ���G0A)G0C�*��0� �� 0B@!A@!`��0G$A� 0Ax P ����G0A� 0B@ A@������ � !_����@��^�^������� � !`������̀draw the energy bar���$�%��.��� � 0f�8�0� ;"#Ad�̲�-@��0@�B�D@� ��@���D�  L�L�D@� ��A���D@ ��  L�V�D@� ��G���D�  L�`��j��t̀draw the score�~����9��&�ߔ�00000 �����E���F��B���DD�P��A��������̀draw the keys������7����Z� � � 0AP���D@�  F��@���D�  h���Z0A�C��� 0D@ �Z!A�"U!E����CY Z��A��� � �  ߔ�00 ��JZ���B��  K�  X�
������(̀animate up ladder�2��<����F� �����P� 0!P�P�Z�@��CV �� $B�(A��B@�  Ax�d��J�n��x0Ax `�0B@��������̀animate down pit�����������Z��������������Z0A�E����0@�J���0�"J�!����B@!��Ax!���G�B@ ��Ax �����B�!����B@!��Ax!���@�B@ ��Ax �����������4"̀-draw outline room type 1 (cave) type 2 (room)�,��
6�����	@ 0!A�J���G�B@�BA�	T��0A�^�@�r��h�L�x���X�q���h�y��r�t�t���o�c���w�O��|�r�@�������@�r����m�z����z�m����r�@�������A@�x����@�C@�Bx���A@�x����G�C@�Bx�������O�̀Hline.draw drawer, support routine for room draw, draws in four quadrants������ � �� �� �0 "B B@� 0 "B Ax�	� ,@�	� /@� &�B@�Ax�� ��B@�Ax�D@!� �,0�B@�Co!Ax��Co! ��B@�Co!Ax�D@!�Co! �:��D� �(�� �N�D@! �(�D@!� �X� �Co!(��Co! �b�D@! �Co!(�D@!�Co! �l��v 0�(0 ��������̀random functions����
��0|��	��0|���������� � �B����0 "e 6"DmZG  "Ii������������
�Z�I���0@��0�RMEYx��Z0A�D� I0�(A�*�0�$A�4�I��0�*�KP@�>��H�� 