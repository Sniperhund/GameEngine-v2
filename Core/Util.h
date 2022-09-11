#pragma once

static const char* shaders[] = {"Unlit", "Lit"};

struct ConvertedMat4
{
    float floats[16];
};

static ConvertedMat4 ConvertMat4ToFloats(glm::mat4 mat)
{
    ConvertedMat4 convertedMat4;
    convertedMat4.floats[0] = mat[0][0];
    convertedMat4.floats[1] = mat[0][1];
    convertedMat4.floats[2] = mat[0][2];
    convertedMat4.floats[3] = mat[0][3];

    convertedMat4.floats[4] = mat[1][0];
    convertedMat4.floats[5] = mat[1][1];
    convertedMat4.floats[6] = mat[1][2];
    convertedMat4.floats[7] = mat[1][3];

    convertedMat4.floats[8] = mat[2][0];
    convertedMat4.floats[9] = mat[2][1];
    convertedMat4.floats[10] = mat[2][2];
    convertedMat4.floats[11] = mat[2][3];

    convertedMat4.floats[12] = mat[3][0];
    convertedMat4.floats[13] = mat[3][1];
    convertedMat4.floats[14] = mat[3][2];
    convertedMat4.floats[15] = mat[3][3];

    return convertedMat4;
}
