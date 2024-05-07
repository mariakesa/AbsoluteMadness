from typing import List
from typing import List, Type
import os
import dotenv

import types


class Kodu:
    def __init__(self):
        self.myfunc = None
        self.foto = "klõps"

    def add_function(self, f):
        self.myfunc = types.MethodType(f, self)


def a(object):
    print(object.foto)


k = Kodu()
k.add_function(a)
k.myfunc()

dotenv.load_dotenv()


class Task:
    def __init__(self):
        self.allen_cache_path = os.environ.get('HGMS_ALLEN_CACHE_PATH')

    def __call__(self, input_text: str, my_func) -> str:
        print('boom!')
        johnny_cash = self.allen_cache_path


class Pipeline:
    def __init__(self, task: List[Type[Task]]):
        self.task = task

    def __call__(self, input_text: str, my_func) -> str:
        output = input_text
        output = self.task()(output, my_func)
        return output


def pipeline(task_name: str) -> Pipeline:
    task_mapping = {
        "neuron-prediction": Task,
    }
    task = task_mapping[task_name]
    return Pipeline(task)


pipeline = pipeline("neuron-prediction")
pipeline("input text", model, regression_model, mixin)


class Pipeline:
    def __init__(self, tasks: List[str]):
        self.tasks = tasks

    def __call__(self, input_text: str) -> str:
        output = input_text
        for task in self.tasks:
            output = self._apply_task(task, output)
        return output

    def _apply_task(self, task: str, input_text: str) -> str:
        # Implement task-specific logic here
        if task == "task1":
            # Task 1 logic
            return input_text
        elif task == "task2":
            # Task 2 logic
            return input_text
        else:
            raise ValueError(f"Invalid task: {task}")


# Example usage
pipeline = Pipeline(tasks=["task1", "task2"])
output = pipeline("input text")
print(output)
