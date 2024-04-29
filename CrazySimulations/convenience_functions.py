import numpy as np


def bin_spikes(spike_monitor, n_neurons, t_total, bin_size):
    # t_total is in seconds
    # bin_size
    spikes_neurons = spike_monitor.i
    spikes_times = spike_monitor.t
    time_series_array = []
    print(np.max(spikes_times))
    bins = np.linspace(0, t_total, int(t_total//bin_size))
    print(len(bins))
    print(bins)
    for neuron in range(n_neurons):
        inds = np.where(spikes_neurons == neuron)
        spike_times_neuron = spikes_times[inds[0]]
        # print(spike_times_neuron)
        binned_ts = np.histogram(spike_times_neuron, bins)[0]
        time_series_array.append(binned_ts)
    time_series_array = np.array(time_series_array)
    return time_series_array
