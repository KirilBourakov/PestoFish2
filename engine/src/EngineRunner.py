from multiprocessing import Process
from time import sleep

class EngineRunner(Process):
    def __init__(self, tasks, results, engine):
        Process.__init__(self)
        self.tasks = tasks
        self.results = results
        self.engine = engine
    
    def run(self):
        while True:
            if (self.tasks.empty()):
                sleep(2)
            else:
                move, dummy, board, color, dummy2 = self.task_queue.get()
                transformed = self.engine.transformer(move, dummy, board, color, dummy2)
                self.tasks.task_done()
                self.results.put(transformed)