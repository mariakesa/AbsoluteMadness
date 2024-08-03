import sys

import itertools
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import joblib as jl
import torch
from sklearn.manifold import TSNE
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.linear_model import LinearRegression
import cebra.datasets
from cebra import CEBRA
import cebra

if torch.cuda.is_available():
    DEVICE = "cuda"
else:
    DEVICE = "cpu"

from aux_funcs import single_session_solver, _compute_emissions_single, allen_frame_id_decode

print(DEVICE)


def run_pipeline(**kwargs):
    cortex = kwargs['cortex']
    num_neurons = kwargs['num_neurons']
    seed = kwargs['seed']
    ca_train = cebra.datasets.init(
        f'allen-movie-one-ca-{cortex}-{num_neurons}-train-10-{seed}')
    ca_test = cebra.datasets.init(
        f'allen-movie-one-ca-{cortex}-{num_neurons}-test-10-{seed}')

    train_steps = 10000
    ca_loader = cebra.data.ContinuousDataLoader(
        ca_train, num_steps=train_steps, batch_size=512, conditional='time_delta', time_offset=1)

    cebra_ca = single_session_solver(data_loader=ca_loader, model_architecture='offset1-model',
                                     distance='cosine', num_hidden_units=128, output_dimension=128,
                                     verbose=True, device=DEVICE, temperature=1, learning_rate=3e-4)

    cebra_ca.fit(ca_loader)
    cebra_ca_emb = _compute_emissions_single(cebra_ca, ca_train)

    fig = plt.figure(figsize=(12, 5))

    ax1 = plt.subplot(121)
    ax1.set_title('Ca')
    ax1.scatter(cebra_ca_emb[:, 0], cebra_ca_emb[:, 1],
                cmap='magma', c=np.tile(np.arange(900), 9), s=1)
    ax1.axis('off')

    plt.show()

    cebra_ca_test = _compute_emissions_single(cebra_ca, ca_test)
    pred_cebra, errs_cebra, acc_cebra = allen_frame_id_decode(cebra_ca_emb, np.tile(
        np.arange(900), 9), cebra_ca_test, np.arange(900), modality='ca', decoder='knn')

    return acc_cebra
