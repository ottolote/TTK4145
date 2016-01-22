import threading
i = 0

mutex_i = threading.Lock() #same as threading.Semaphore(), but Semaphore was HORRIBLY SLOW. Lock() is not really much better

def func1():
	global i
	for x in range(1000000):
        	mutex_i.acquire()
		i += 1
                mutex_i.release()

def func2():
	global i
	for x in range(1000000):
        	mutex_i.acquire()
		i -= 1
                mutex_i.release()

thread1 = threading.Thread(name='func1', target=func1)
thread2 = threading.Thread(name='func2', target=func2)

thread1.start()
thread2.start()
thread1.join()
thread2.join()
print(i)
