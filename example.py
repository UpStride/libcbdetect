import cbdetect_python as cb
import cv2 as cv

img = cv.imread('example_data/e2.png')
checkerboards = cb.detect(img)
print(f'There is a total of {len(checkerboards)} checkerboards detected in the input image.')
print(f'The shape of the first checkerboard is {checkerboards[0].shape}.')
