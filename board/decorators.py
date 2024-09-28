def wait_for_promotion(func):
    def wrapper(self, *args, **kwargs):
        if not self.waiting_for_promotion:
            func(self, *args, **kwargs)
    return wrapper