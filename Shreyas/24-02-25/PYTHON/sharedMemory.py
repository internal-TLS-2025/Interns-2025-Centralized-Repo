from multiprocessing import shared_memory, Process
import time

def writer():
    shm = shared_memory.SharedMemory(create=True, size=1024, name='shared_mem')
    shm.buf[:5] = b'Hello'
    time.sleep(5)  # Keep memory available for reading
    shm.close()  # Close shared memory (DO NOT unlink)

def reader():
    time.sleep(1)  # Ensure writer creates memory first
    shm = shared_memory.SharedMemory(name='shared_mem')
    print(bytes(shm.buf[:5]).decode())  # Read shared memory
    shm.close()  # Close (DO NOT unlink)

if __name__ == "__main__":
    p1 = Process(target=writer)
    p2 = Process(target=reader)

    p1.start()
    p2.start()

    p1.join()
    p2.join()

    # Only unlink in the main process after child processes finish
    try:
        shm = shared_memory.SharedMemory(name='shared_mem')
        shm.unlink()  # Cleanup safely
    except FileNotFoundError:
        print("Shared memory already unlinked by Python’s resource tracker.")

    print("Shared memory cleanup complete.")

