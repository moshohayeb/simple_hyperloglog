import redis
import random
import string

'''
Just a test script, generate random strings
python generate.py > TEST.txt
./hyperloglog TEST.txt
'''

#r = redis.Redis(host='localhost', port=6379, db=0)

for i in range(1000000):
	chars = "".join( [random.choice(string.letters).lower() for i in xrange(3)] )
	print chars
	#r.execute_command('PFADD', 'mkey', chars)





