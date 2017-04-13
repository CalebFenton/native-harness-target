#!/usr/bin/env python

import socket


HOST_IP = '127.0.0.1'
PORT_NO = 5001
BUFFER_SIZE = 2048

ENC_STR_BYTES = [40, 65, 24, 16, 4, 27, 9, 65, 24, 31, 75, 16, 91, 69, 19, 31, 4, 31, 30, 71, 18, 22, 65, 93, 24, 69, 13, 15, 77, 11, 30, 4, 18, 29, 4, 4, 20, 81, 15, 91, 64, 24, 8, 77, 15, 30, 87, 83, 91, 119, 24, 30, 79, 93, 31, 77, 14, 24, 77, 13, 23, 77, 19, 30, 4, 28, 21, 64, 93, 29, 77, 19, 31, 4, 4, 20, 81, 15, 91, 72, 20, 25, 65, 15, 15, 93, 83]

# Be sure to run forward ports from emulator with:
# adb forward tcp:5001 tcp:5001
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST_IP, PORT_NO))

print 'Sending encrypted string'
enc_str = "".join(map(chr, ENC_STR_BYTES))
s.sendall('{}\n'.format(enc_str))

data = s.recv(BUFFER_SIZE)
s.close()


print("Decrypted string: \"{}\"".format(data))