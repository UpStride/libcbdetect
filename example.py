import cbdetect_python as cb
import numpy as np
import cv2 as cv

img = cv.imread('example_data/e2.png')
checkerboards = cb.detect(img)
print(f'There is a total of {len(checkerboards)} checkerboards detected in the input image.')
print(f'The shape of the first checkerboard is {checkerboards[0].shape}.')

# Superpose the checkerboard lines on the image
for board in checkerboards:
    corners = np.reshape(board, (-1, 1, 2)).astype(np.float32)
    img = cv.drawChessboardCorners(img, board.shape[0:2], corners, True)

# Draw and display the corners
while(1):
    cv.imshow('img2', cv.pyrDown(img))
    k = cv.waitKey(33)
    if k == 27 or k == ord('q'):  # Esc or 'q' keys to stop
        break

cv.destroyAllWindows()
