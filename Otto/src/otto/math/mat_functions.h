#pragma once

#include "otto/base.h"
#include "otto/math/mat.h"
#include "otto/math/mat2x2.h"
#include "otto/math/mat3x3.h"
#include "otto/math/mat4x4.h"

namespace otto
{
    template<typename T>
    static T determinant(const Mat<2, 2, T>& mat)
    {
        return mat.m11 * mat.m22 - mat.m12* mat.m21;
    }

    template<typename T>
    static T determinant(const Mat<3, 3, T>& mat)
    {
        return mat.mCols[0][0] * (mat.mCols[1][1] * mat.mCols[2][2] - mat.mCols[2][1] * mat.mCols[1][2]) -
               mat.mCols[1][0] * (mat.mCols[0][1] * mat.mCols[2][2] - mat.mCols[2][1] * mat.mCols[0][2]) +
               mat.mCols[2][0] * (mat.mCols[0][1] * mat.mCols[1][2] - mat.mCols[1][1] * mat.mCols[0][2]);
    }

    template<typename T>
    static T determinant(const Mat<4, 4, T>& mat)
    {
        T subFactor0 = mat.mCols[2][2] * mat.mCols[3][3] - mat.mCols[3][2] * mat.mCols[2][3];
        T subFactor1 = mat.mCols[2][1] * mat.mCols[3][3] - mat.mCols[3][1] * mat.mCols[2][3];
        T subFactor2 = mat.mCols[2][1] * mat.mCols[3][2] - mat.mCols[3][1] * mat.mCols[2][2];
        T subFactor3 = mat.mCols[2][0] * mat.mCols[3][3] - mat.mCols[3][0] * mat.mCols[2][3];
        T subFactor4 = mat.mCols[2][0] * mat.mCols[3][2] - mat.mCols[3][0] * mat.mCols[2][2];
        T subFactor5 = mat.mCols[2][0] * mat.mCols[3][1] - mat.mCols[3][0] * mat.mCols[2][1];

        Vec<4, T> detCof = {
             (mat.mCols[1][1] * subFactor0 - mat.mCols[1][2] * subFactor1 + mat.mCols[1][3] * subFactor2),
            -(mat.mCols[1][0] * subFactor0 - mat.mCols[1][2] * subFactor3 + mat.mCols[1][3] * subFactor4),
             (mat.mCols[1][0] * subFactor1 - mat.mCols[1][1] * subFactor3 + mat.mCols[1][3] * subFactor5),
            -(mat.mCols[1][0] * subFactor2 - mat.mCols[1][1] * subFactor4 + mat.mCols[1][2] * subFactor5),
        };

        return
            mat.mCols[0][0] * detCof[0] + mat.mCols[0][1] * detCof[1] +
            mat.mCols[0][2] * detCof[2] + mat.mCols[0][3] * detCof[3];
    }

    template<typename T>
    static Mat<2, 2, T> inverse(const Mat<2, 2, T>& mat)
    {
        T oneOverDeterminant = T(1) / determinant(mat);

        return {
             mat.m22 * oneOverDeterminant,
            -mat.m12 * oneOverDeterminant,
            -mat.m21 * oneOverDeterminant,
             mat.m11 * oneOverDeterminant,
        };
    }

    template<typename T>
    static Mat<3, 3, T> inverse(const Mat<3, 3, T>& mat)
    {
        T oneOverDeterminant = T(1) / determinant(mat);

        Mat<3, 3, T> inverse;
        inverse.mCols[0][0] = +(mat.mCols[1][1] * mat.mCols[2][2] - mat.mCols[2][1] * mat.mCols[1][2]) * oneOverDeterminant;
        inverse.mCols[1][0] = -(mat.mCols[1][0] * mat.mCols[2][2] - mat.mCols[2][0] * mat.mCols[1][2]) * oneOverDeterminant;
        inverse.mCols[2][0] = +(mat.mCols[1][0] * mat.mCols[2][1] - mat.mCols[2][0] * mat.mCols[1][1]) * oneOverDeterminant;
        inverse.mCols[0][1] = -(mat.mCols[0][1] * mat.mCols[2][2] - mat.mCols[2][1] * mat.mCols[0][2]) * oneOverDeterminant;
        inverse.mCols[1][1] = +(mat.mCols[0][0] * mat.mCols[2][2] - mat.mCols[2][0] * mat.mCols[0][2]) * oneOverDeterminant;
        inverse.mCols[2][1] = -(mat.mCols[0][0] * mat.mCols[2][1] - mat.mCols[2][0] * mat.mCols[0][1]) * oneOverDeterminant;
        inverse.mCols[0][2] = +(mat.mCols[0][1] * mat.mCols[1][2] - mat.mCols[1][1] * mat.mCols[0][2]) * oneOverDeterminant;
        inverse.mCols[1][2] = -(mat.mCols[0][0] * mat.mCols[1][2] - mat.mCols[1][0] * mat.mCols[0][2]) * oneOverDeterminant;
        inverse.mCols[2][2] = +(mat.mCols[0][0] * mat.mCols[1][1] - mat.mCols[1][0] * mat.mCols[0][1]) * oneOverDeterminant;

        return inverse;
    }

    template<typename T>
    static Mat<4, 4, T> inverse(const Mat<4, 4, T>& mat)
    {
        T coef00 = mat.mCols[2][2] * mat.mCols[3][3] - mat.mCols[3][2] * mat.mCols[2][3];
        T coef02 = mat.mCols[1][2] * mat.mCols[3][3] - mat.mCols[3][2] * mat.mCols[1][3];
        T coef03 = mat.mCols[1][2] * mat.mCols[2][3] - mat.mCols[2][2] * mat.mCols[1][3];

        T coef04 = mat.mCols[2][1] * mat.mCols[3][3] - mat.mCols[3][1] * mat.mCols[2][3];
        T coef06 = mat.mCols[1][1] * mat.mCols[3][3] - mat.mCols[3][1] * mat.mCols[1][3];
        T coef07 = mat.mCols[1][1] * mat.mCols[2][3] - mat.mCols[2][1] * mat.mCols[1][3];

        T coef08 = mat.mCols[2][1] * mat.mCols[3][2] - mat.mCols[3][1] * mat.mCols[2][2];
        T coef10 = mat.mCols[1][1] * mat.mCols[3][2] - mat.mCols[3][1] * mat.mCols[1][2];
        T coef11 = mat.mCols[1][1] * mat.mCols[2][2] - mat.mCols[2][1] * mat.mCols[1][2];

        T coef12 = mat.mCols[2][0] * mat.mCols[3][3] - mat.mCols[3][0] * mat.mCols[2][3];
        T coef14 = mat.mCols[1][0] * mat.mCols[3][3] - mat.mCols[3][0] * mat.mCols[1][3];
        T coef15 = mat.mCols[1][0] * mat.mCols[2][3] - mat.mCols[2][0] * mat.mCols[1][3];

        T coef16 = mat.mCols[2][0] * mat.mCols[3][2] - mat.mCols[3][0] * mat.mCols[2][2];
        T coef18 = mat.mCols[1][0] * mat.mCols[3][2] - mat.mCols[3][0] * mat.mCols[1][2];
        T coef19 = mat.mCols[1][0] * mat.mCols[2][2] - mat.mCols[2][0] * mat.mCols[1][2];

        T coef20 = mat.mCols[2][0] * mat.mCols[3][1] - mat.mCols[3][0] * mat.mCols[2][1];
        T coef22 = mat.mCols[1][0] * mat.mCols[3][1] - mat.mCols[3][0] * mat.mCols[1][1];
        T coef23 = mat.mCols[1][0] * mat.mCols[2][1] - mat.mCols[2][0] * mat.mCols[1][1];

        Vec<4, T> fac0 = { coef00, coef00, coef02, coef03 };
        Vec<4, T> fac1 = { coef04, coef04, coef06, coef07 };
        Vec<4, T> fac2 = { coef08, coef08, coef10, coef11 };
        Vec<4, T> fac3 = { coef12, coef12, coef14, coef15 };
        Vec<4, T> fac4 = { coef16, coef16, coef18, coef19 };
        Vec<4, T> fac5 = { coef20, coef20, coef22, coef23 };

        Vec<4, T> vec0 = { mat.mCols[1][0], mat.mCols[0][0], mat.mCols[0][0], mat.mCols[0][0] };
        Vec<4, T> vec1 = { mat.mCols[1][1], mat.mCols[0][1], mat.mCols[0][1], mat.mCols[0][1] };
        Vec<4, T> vec2 = { mat.mCols[1][2], mat.mCols[0][2], mat.mCols[0][2], mat.mCols[0][2] };
        Vec<4, T> vec3 = { mat.mCols[1][3], mat.mCols[0][3], mat.mCols[0][3], mat.mCols[0][3] };

        Vec<4, T> inv0 = { 
            vec1[0] * fac0[0] - vec2[0] * fac1[0] + vec3[0] * fac2[0],
            vec1[1] * fac0[1] - vec2[1] * fac1[1] + vec3[1] * fac2[1],
            vec1[2] * fac0[2] - vec2[2] * fac1[2] + vec3[2] * fac2[2],
            vec1[3] * fac0[3] - vec2[3] * fac1[3] + vec3[3] * fac2[3],
        };

        Vec<4, T> inv1 = { 
            vec0[0] * fac0[0] - vec2[0] * fac3[0] + vec3[0] * fac4[0],
            vec0[1] * fac0[1] - vec2[1] * fac3[1] + vec3[1] * fac4[1],
            vec0[2] * fac0[2] - vec2[2] * fac3[2] + vec3[2] * fac4[2],
            vec0[3] * fac0[3] - vec2[3] * fac3[3] + vec3[3] * fac4[3],
        };

        Vec<4, T> inv2 = { 
            vec0[0] * fac1[0] - vec1[0] * fac3[0] + vec3[0] * fac5[0],
            vec0[1] * fac1[1] - vec1[1] * fac3[1] + vec3[1] * fac5[1],
            vec0[2] * fac1[2] - vec1[2] * fac3[2] + vec3[2] * fac5[2],
            vec0[3] * fac1[3] - vec1[3] * fac3[3] + vec3[3] * fac5[3],
        };

        Vec<4, T> inv3 = { 
            vec0[0] * fac2[0] - vec1[0] * fac4[0] + vec2[0] * fac5[0],
            vec0[1] * fac2[1] - vec1[1] * fac4[1] + vec2[1] * fac5[1],
            vec0[2] * fac2[2] - vec1[2] * fac4[2] + vec2[2] * fac5[2],
            vec0[3] * fac2[3] - vec1[3] * fac4[3] + vec2[3] * fac5[3],
        };

        Mat<4, 4, T> inverse = {
            Vec<4, T>{  inv0[0], -inv0[1],  inv0[2], -inv0[3], },
            Vec<4, T>{ -inv1[0],  inv1[1], -inv1[2],  inv1[3], },
            Vec<4, T>{  inv2[0], -inv2[1],  inv2[2], -inv2[3], },
            Vec<4, T>{ -inv3[0],  inv3[1], -inv3[2],  inv3[3], },
        };

        Vec<4, T> row0 = { inverse.mCols[0][0], inverse.mCols[1][0], inverse.mCols[2][0], inverse.mCols[3][0] };

        Vec<4, T> dot0 = { 
            mat.mCols[0][0] * row0[0], 
            mat.mCols[0][1] * row0[1], 
            mat.mCols[0][2] * row0[2], 
            mat.mCols[0][3] * row0[3] 
        };

        T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
        T oneOverDeterminant = T(1) / dot1;

        return inverse * oneOverDeterminant;
    }

    template<typename T>
    static Mat<2, 2, T> transpose(const Mat<2, 2, T>& mat)
    {
        return {
            mat.m11, mat.m21,
            mat.m12, mat.m22,
        };
    }

    template<typename T>
    static Mat<3, 3, T> transpose(const Mat<3, 3, T>& mat)
    {
        return {
            mat.m11, mat.m21, mat.m31,
            mat.m12, mat.m22, mat.m32,
            mat.m13, mat.m23, mat.m33,
        };
    }

    template<typename T>
    static Mat<4, 4, T> transpose(const Mat<4, 4, T>& mat)
    {
        return {
            mat.m11, mat.m21, mat.m31, mat.m41,
            mat.m12, mat.m22, mat.m32, mat.m42,
            mat.m13, mat.m23, mat.m33, mat.m43,
            mat.m14, mat.m24, mat.m34, mat.m44,
        };
    }

} // namespace otto
