from multiprocessing import shared_memory
import numpy as np

# Create a shared memory block
shm = shared_memory.SharedMemory(create=True, size=1024)

# Convert shared memory into a NumPy array
data = np.ndarray((10,), dtype=np.int32, buffer=shm.buf)
data[:] = np.arange(10)  # Write some numbers

print(f"Process 1 wrote: {data.tolist()}")
print(f"Shared memory name: {shm.name}")  # Pass this name to Process 2

# Keep running so Process 2 can access it
input("Press Enter to clean up...")

# Cleanup
shm.close()
shm.unlink()


