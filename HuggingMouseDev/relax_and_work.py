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
        return self

    def plot(self, args=None):
        print('plotting')
        return self

    def filter_data(self, args=None):
        print('filtering')
        return self


def pipeline(task_name: str, **kwargs) -> Pipeline:
    task_mapping = {
        "neuron-prediction": NeuronPredictionPipeline,
    }
    task = task_mapping[task_name]
    return task(**kwargs)


pipe = pipeline("neuron-prediction", model='car',
                regression_model='house', analysis_function='tree')

pipe(1234).filter_data('my_filter').plot('my_func')
