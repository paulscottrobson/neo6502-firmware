10 t1 = time()
11 print "Start"
20 k=0
30 k=k+1
40 LET a=k*k
45 LET b=LOG(k)
47 LET c=SIN(k)
50 IF k<2000 THEN GOTO 30
60 print "End"
70 print (time()-t1)/100/20:end
700 return
