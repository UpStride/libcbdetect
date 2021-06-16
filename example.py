import cbdetect_python as cb
import numpy as np

checkerboards = cb.detect('example_data/e2.png')
checkerboards = [np.asarray(checkerboard) for checkerboard in checkerboards]
print(f'There is a total of {len(checkerboards)} detected in the input image.')
print(f'The shape of the first checkerboard is {checkerboards[0].shape}.')