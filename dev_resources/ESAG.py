import numpy as np
from scipy import optimize
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def _sample_from_ESAG(mu_1, mu_2, mu_3, gamma_1, gamma_2, size):

    #first compute inverse covariance matrix

    #xi1 and xi2 (eq. 14)

    mu = np.array([mu_1, mu_2, mu_3])

    mu_0 = np.sqrt(mu_2 * mu_2 + mu_3 * mu_3)

    #print(mu_0)

    norm_mu = np.linalg.norm(mu)
    #print(norm_mu)
    xi_1 = np.array([-mu_0 * mu_0, mu_1 * mu_2, mu_1*mu_3])/(mu_0 * norm_mu)

    xi_2 = np.array([0, -mu_3, mu_2])/mu_0

    first_bracket = np.outer(xi_1, xi_1.T) - np.outer(xi_2, xi_2.T)
    second_bracket = np.outer(xi_1, xi_2.T) + np.outer(xi_2, xi_1.T)
    factor = np.sqrt(gamma_1 * gamma_1 + gamma_2 * gamma_2 +1) -1
    third_bracket = np.outer(xi_1, xi_1.T) + np.outer(xi_2, xi_2.T)

    inv_cov = np.eye(3) + gamma_1 * first_bracket + gamma_2 * second_bracket + factor * third_bracket

    #print(inv_cov)

    cov = np.linalg.inv(inv_cov)

    unnormalized_samples = np.random.multivariate_normal(mu, cov, size)
    
    #print(unnormalized_samples)

    norms = np.linalg.norm(unnormalized_samples, axis=1)[:,np.newaxis]

    #print(norms)
    samples = unnormalized_samples/norms

    return samples

samples = _sample_from_ESAG(-4, -8, 8, 1, -1, 50000)

#print(samples)

lats = 30
longs = 30

u = np.linspace( 0, 2 * np.pi, longs)
v = np.linspace( 0, np.pi, lats)

# create the sphere surface
XX = np.outer( np.cos( u ), np.sin( v ) )
YY = np.outer( np.sin( u ), np.sin( v ) )
ZZ = np.outer( np.ones( np.size( u ) ), np.cos( v ) )

fig = plt.figure()
ax = fig.add_subplot( 1, 1, 1, projection='3d')

ax.plot_surface( XX, YY,  ZZ)

ax.scatter(samples[:,0],samples[:,1],samples[:,2],c='r',s=3)
plt.show()
#class ESAG(mu, gamma_1, gamma_2):


#    self.mu = mu