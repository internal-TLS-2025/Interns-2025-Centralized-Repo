from multiprocessing import shared_memory

import numpy as np



# Connect to the existing shared memory (use the name printed by Process 1)

shm = shared_memory.SharedMemory(name="psm_a3ff6f74") 



# Access shared memory as a NumPy array

data = np.ndarray((10,), dtype=np.int32, buffer=shm.buf)



print(f"Process 2 read: {data.tolist()}")



# Cleanup

shm.close()

