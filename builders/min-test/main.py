from multiprocessing import Process, JoinableQueue, Queue, cpu_count
from time import sleep

class Runner(Process):
    def __init__(self, tasks, results, obj):
        Process.__init__(self)
        self.tasks = tasks
        self.results = results

    def run(self):
        while not self.tasks.empty():
            x = self.tasks.get()
            x = x * 2
            self.tasks.task_done()
            self.results.put(x)

class Owner():
    def __init__(self):  
        self.tasks = JoinableQueue()
        self.results = Queue()
        self.runners = [Runner(self.tasks, self.results, self) for i in range(cpu_count())]
        for runner in self.runners:
            runner.start()

    def op(self, inp):
        return inp*2

    def process(self, tasks):
        for task in tasks:
            self.tasks.put(task)

        for runner in self.runners:
            runner.run()
        
        self.tasks.join()

        while not self.results.empty():
            print(self.results.get())

def main():
    owner = Owner()
    owner.process([1,2,3,4,5,6,7,8,9,10])

if __name__ == '__main__':
    main()
