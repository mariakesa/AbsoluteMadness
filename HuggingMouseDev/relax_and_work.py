from abc import abstractmethod


class Pipeline:
    @abstractmethod
    def __call__(self, experiment_id) -> str:
        pass


class NeuronPredictionPipeline(Pipeline):
    def __init__(self, **kwargs):
        self.model = kwargs['model']
        self.regression_model = kwargs['regression_model']
        self.analysis_function = kwargs['analysis_function']

    def __call__(self, experiment_id) -> str:
        output = experiment_id
        print(output)
        return output


def pipeline(task_name: str, **kwargs) -> Pipeline:
    task_mapping = {
        "neuron-prediction": NeuronPredictionPipeline,
    }
    task = task_mapping[task_name]
    return task(**kwargs)


pipe = pipeline("neuron-prediction", model='car',
                regression_model='house', analysis_function='tree')

pipe(1234)
