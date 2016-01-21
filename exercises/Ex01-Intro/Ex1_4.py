import threading
i = 0

def func1():
	global i
	for x in range(1000000):
		i += 1

def func2():
	global i
	for x in range(1000000):
		i -= 1

thread1 = threading.Thread(name='func1', target=func1)
thread2 = threading.Thread(name='func2', target=func2)

thread1.start()
thread2.start()
print(i)
