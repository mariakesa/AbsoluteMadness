from typing import List
from typing import List, Type


class Task:
    def __call__(self, input_text: str) -> str:
        raise NotImplementedError


class Task1(Task):
    def __call__(self, input_text: str) -> str:
        # Task 1 logic
        return input_text


class Task2(Task):
    def __call__(self, input_text: str) -> str:
        # Task 2 logic
        return input_text


class Pipeline:
    def __init__(self, tasks: List[Type[Task]]):
        self.tasks = tasks

    def __call__(self, input_text: str) -> str:
        output = input_text
        for task in self.tasks:
            output = task()(output)
        return output


def pipeline(task_names: List[str]) -> Pipeline:
    task_mapping = {
        "task1": Task1,
        "task2": Task2,
    }
    tasks = [task_mapping[task_name] for task_name in task_names]
    return Pipeline(tasks)


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
