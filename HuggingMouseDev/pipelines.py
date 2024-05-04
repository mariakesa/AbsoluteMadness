from typing import List


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
