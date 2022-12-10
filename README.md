A collection of random components for long-term programming competitions. 
Another dedicated NN library repository is [here](https://github.com/nanaeda/nn_eda).

## List of components

### Weighted Random Element Sampler

We can pick up an element from a list of elements.  
An element is picked up with the probabiity of (weights\[i\] / sum(weights)).

### Timer

A very thin wrapper over chrono. 

### Numbered Files

A simple file manager for reinforcement learning.
It just holds files numbered from 0 to n with potentially missing ones.
We can get latest k files or create a new one numbered with (n + 1).
The file creation is thread-safe. 


