#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  2 15:42:13 2020

@author: tyrion
"""
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# def random_vectors(size, phi, sigma_phi, alpha, sigma_alpha):
#     """
#     size: number of vectors
#     phi: in-plane angle
#     sigma_phi: standard deviation in plane
#     alpha: out-of-plane angle
#     sigma_alpha: standard deviation out-of-plane
#
#     Returns:
#     vectors as ndarray [size, 3]
#     """
#     phi_samples = np.random.normal(phi, sigma_phi, size)
#     alpha_samples = np.random.normal(alpha, sigma_alpha, size)
#
#     vectors = np.empty((size, 3))
#
#     vectors[:, 0] = np.sin(alpha_samples) * np.cos(phi_samples)
#     vectors[:, 1] = np.sin(alpha_samples) * np.sin(phi_samples)
#     vectors[:, 2] = np.cos(alpha_samples)
#     return vectors
#
# # vecs = random_vectors(3000, 0.1, 1, 1, 0.01)
# # print(vecs)
# # np.save('../test_data/test3', vecs)
#
#
# vecs2 = [[1, 0, 0], [0, 1, 0], [0, 0, 1], [1, 1, 1],  [1, -1, -1]]
# print(vecs2)
# npvecs2 = np.array(vecs2, dtype=np.double)
# print(npvecs2)
# np.save('../test_data/punkteTest', npvecs2)

vecs = np.load('../test_data/1.npy')
fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection = '3d')
ax.scatter(vecs[:, 0], vecs[:, 1], vecs[:, 2])

plt.show()
