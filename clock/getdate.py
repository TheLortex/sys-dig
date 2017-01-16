import time
h = int(time.strftime("%H")) 
m = int(time.strftime("%M"))
s = int(time.strftime("%S"))
D = int(time.strftime("%d"))
M = int(time.strftime("%m"))
Y = int(time.strftime("%Y"))


r0 = s%10 + ((s/10) << 4) + ((m%10) << 7) + ((m/10) << 11) + ((h%10) << 14) + ((h/10) << 18) + ((D%10) << 20) + ((D/10) << 24) + ((M%10) << 26) + ((M/10) << 30)

r1 = Y%10 + (((Y/10)%10) << 4) + (((Y/100)%10) << 8) + (((Y/1000)%10) << 12) + ((Y%4) << 30)

r2 = 0x327
r3 = r2 + 0x1000
r4 = r3 + 0x18000
r5 = r3 + 0x70000
r6 = r5 + 0x600000
r7 = 499 + 4*((Y/1000)%10)
r8 = 479 + 2*((Y/100)%10)
r9 = 458 + 2*((Y/10)%10)
rA = 436 + 2*(Y%10)
if (Y % 4 == 0 and ((Y%100 != 0) or (Y%100 == 0 and Y%400 == 0)) and M <= 2):
    if D <= 27:
        rB = 382 + 3*(M-1)
    else:
        rB = 383 + 3*(M-1)
elif (M <= 9):
    if D <= 27:
        rB = 390 + 3*(M-1)
    else:
        rB = 391 + 3*(M-1)
else:
    if D <= 27:
        rB = 418 + 3*(M-10)
    else:
        rB = 419 + 3*(M-10)
if D <= 28:
    rC = 301 + (D-1)*2
elif D == 29 and M == 2:
    rC = 362
elif (D == 29 or D == 30) and (M == 4 or M == 6 or M == 9 or M == 11):
    rC = 367 + (D-29)*2
else:
    rC = 372 + (D-29)*2

rD = 252 + h*2 
rE = 131 + m*2 
rF = 10 + s*2


r0 = "{0:b}".format(r0)
r1 = "{0:b}".format(r1)
r2 = "{0:b}".format(r2)
r3 = "{0:b}".format(r3)
r4 = "{0:b}".format(r4)
r5 = "{0:b}".format(r5)
r6 = "{0:b}".format(r6)
r7 = "{0:b}".format(r7)
r8 = "{0:b}".format(r8)
r9 = "{0:b}".format(r9)
rA = "{0:b}".format(rA)
rB = "{0:b}".format(rB)
rC = "{0:b}".format(rC)
rD = "{0:b}".format(rD)
rE = "{0:b}".format(rE)
rF = "{0:b}".format(rF)


print(r0)
print(r1)
print(r2)
print(r3)
print(r4)
print(r5)
print(r6)
print(r7)
print(r8)
print(r9)
print(rA)
print(rB)
print(rC)
print(rD)
print(rE)
print(rF)
