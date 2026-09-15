#pragma once

#include <raylib.h>
#include <vector>

class PixelUtils {
public:
    static bool isPopulated(int alpha);

    // This may not work if the first pixel is entered from the last pixel of the row above it.
    static std::vector<int> getBoundaryPixelIndexes();

    static void createBodyFromBoundaryPixelIndexes(std::vector<int> boundaryPixelIndexes);

private:
    static std::vector<int> getAdjacentPixelIndexes(int currentPixelIndex, Vector2 imageSizePixels);

    static int getPreviousPixelAdjacentIndex(int                     previousPixelIndex,
                                             const std::vector<int> &adjacentPixelIndexes);

    static int findNextBoundaryPixel(std::vector<bool> pixelsIsPopulated,
                                     Vector2           imageSizePixels,
                                     int               previousPixelIndex,
                                     int               currentPixelIndex);

    static int findFirstPopulatedPixelIndex(std::vector<bool> pixelsIsPopulated);
};
