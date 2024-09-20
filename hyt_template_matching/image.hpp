#ifndef IMAGE_HPP
#define IMAGE_HPP

#include<stdexcept>
#include<vector>

#include"constants.h"

class Image {
public:
    
    class Row {
    public:
        Row(unsigned char * rowPtr, int width) : rowPtr(rowPtr), width(width) {}

        unsigned char& operator[](int col) {
            if (col < 0 || col >= width) {
                throw std::out_of_range("Index out of bounds");
            }
            return rowPtr[col];
        }

    private:
        unsigned char* rowPtr;  
        int width; 
    };


    class ConstRow {
    public:
        ConstRow(const unsigned char * rowPtr, int width) : rowPtr(rowPtr), width(width) {}

        const unsigned char& operator[](int col) const {
            if (col < 0 || col >= width) {
                throw std::out_of_range("Index out of bounds");
            }
            return rowPtr[col];
        }

    private:
        const unsigned char* rowPtr;
        int width;
    };

    int height, width;


    Image() : Image(0, 0) {}


    Image(int height, int width) : height(height), width(width), data(height* width) {}

    unsigned char& operator[](std::pair<int, int> position) {
        int row = position.first;
        int col = position.second;
        if (row < 0 || row >= height || col < 0 || col >= width) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row * width + col];
    }


    const unsigned char& operator[](std::pair<int, int> position) const {
        int row = position.first;
        int col = position.second;
        if (row < 0 || row >= height || col < 0 || col >= width) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row * width + col];
    }

    Row operator[](int rowIndex) {
        if (rowIndex < 0 || rowIndex >= height) {
            throw std::out_of_range("Index out of bounds");
        }
        return Row(data.data() + rowIndex * width, width);
    }

    ConstRow operator[](int rowIndex) const {
        if (rowIndex < 0 || rowIndex >= height) {
            throw std::out_of_range("Index out of bounds");
        }
        return ConstRow(data.data() + rowIndex * width, width);
    }

private:
    std::vector<unsigned char> data;
};

#endif

