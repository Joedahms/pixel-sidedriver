#include "PixelUtils.hpp"

#include <iostream>

bool PixelUtils::isPopulated(const int alpha) {
    if (alpha > 0) { return true; }
    return false;
}

// This may not work if the first pixel is entered from the last pixel of the row above it.
std::vector<int> PixelUtils::getBoundaryPixelIndexes() {
    const Image  wheelImage      = LoadImage("sprites/wheel.png");
    Color *colors          = LoadImageColors(wheelImage);
    const int    imageSizePixels = wheelImage.width * wheelImage.height;

    std::vector<bool> pixelsIsPopulated;
    for (int i = 0; i < imageSizePixels; i++) {
        pixelsIsPopulated.push_back(isPopulated(colors[i].a));
    }

    std::vector<int> boundaryPixelIndexes;

    int firstBoundaryPixelIndex = findFirstPopulatedPixelIndex(pixelsIsPopulated);
    boundaryPixelIndexes.push_back(firstBoundaryPixelIndex);

    // TODO: Work with boundaries that may hit the start more than once.
    int previousBoundaryPixelIndex = firstBoundaryPixelIndex - 1;
    int currentBoundaryPixelIndex  = firstBoundaryPixelIndex;
    int timesFirstPixelFound       = 0;
    while (true) {
        int nextBoundaryPixelIndex = findNextBoundaryPixel(pixelsIsPopulated,
                                                           {
                                                               static_cast<float>(wheelImage.width),
                                                               static_cast<float>(wheelImage.height)
                                                           },
                                                           previousBoundaryPixelIndex,
                                                           currentBoundaryPixelIndex);
        if (nextBoundaryPixelIndex == firstBoundaryPixelIndex) {
            timesFirstPixelFound++;
            if (timesFirstPixelFound == 2) { break; }
        }
        boundaryPixelIndexes.push_back(nextBoundaryPixelIndex);
        previousBoundaryPixelIndex = currentBoundaryPixelIndex;
        currentBoundaryPixelIndex  = nextBoundaryPixelIndex;
    }

    UnloadImage(wheelImage);
    UnloadImageColors(colors);
    for (const int boundaryPixelIndex: boundaryPixelIndexes) {
        std::cout << boundaryPixelIndex << std::endl;
    }
    return boundaryPixelIndexes;
}

std::vector<int> PixelUtils::getAdjacentPixelIndexes(const int     currentPixelIndex,
                                                     const Vector2 imageSizePixels) {
    std::vector<int> adjacentPixelIndexes;

    auto isTop = [currentPixelIndex, imageSizePixels] {
        return currentPixelIndex < imageSizePixels.x;
    };
    auto isLeft = [currentPixelIndex, imageSizePixels] {
        return currentPixelIndex % static_cast<int>(imageSizePixels.x) == 0;
    };
    auto isRight = [currentPixelIndex, imageSizePixels] {
        return (currentPixelIndex + 1) % static_cast<int>(imageSizePixels.x) == 0;
    };
    auto isBottom = [currentPixelIndex, imageSizePixels] {
        return currentPixelIndex >= imageSizePixels.x * (imageSizePixels.y - 1);
    };

    if (!(isTop() || isLeft())) {
        adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x - 1);
    }
    if (!isTop()) { adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x); }
    if (!(isTop() || isRight())) {
        adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x + 1);
    }
    if (!isRight()) { adjacentPixelIndexes.push_back(currentPixelIndex + 1); }
    if (!(isRight() || isBottom())) {
        adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x + 1);
    }
    if (!isBottom()) { adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x); }
    if (!(isBottom() || isLeft())) {
        adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x - 1);
    }
    if (!isLeft()) { adjacentPixelIndexes.push_back(currentPixelIndex - 1); }

    return adjacentPixelIndexes;
}

int PixelUtils::getPreviousPixelAdjacentIndex(const int               previousPixelIndex,
                                              const std::vector<int> &adjacentPixelIndexes) {
    for (int i = 0; i < adjacentPixelIndexes.size(); i++) {
        if (previousPixelIndex == adjacentPixelIndexes[i]) { return i; }
    }
    return -1;
}

int PixelUtils::findNextBoundaryPixel(std::vector<bool> pixelsIsPopulated,
                                      const Vector2     imageSizePixels,
                                      int               previousPixelIndex,
                                      int               currentPixelIndex) {
    const std::vector<int> adjacentPixelIndexes = getAdjacentPixelIndexes(currentPixelIndex,
        imageSizePixels);
    int previousPixelAdjacentIndex = getPreviousPixelAdjacentIndex(previousPixelIndex,
        adjacentPixelIndexes);
    previousPixelAdjacentIndex++;

    int examinedPixelAdjacentIndex = previousPixelAdjacentIndex;
    while (true) {
        if (examinedPixelAdjacentIndex == adjacentPixelIndexes.size()) {
            examinedPixelAdjacentIndex = 0;
        }
        if (pixelsIsPopulated[adjacentPixelIndexes[examinedPixelAdjacentIndex]]) {
            return adjacentPixelIndexes[examinedPixelAdjacentIndex];
        }
        examinedPixelAdjacentIndex++;
    }
}

int PixelUtils::findFirstPopulatedPixelIndex(std::vector<bool> pixelsIsPopulated) {
    for (int i = 0; i < pixelsIsPopulated.size(); i++) { if (pixelsIsPopulated[i]) { return i; } }
    return -1; // TODO: Picture empty?
}
