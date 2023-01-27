/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TERRAIN_TECHNIQUE_H
#define TERRAIN_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class TerrainTechnique : public Technique
{
public:

    TerrainTechnique();

    virtual bool Init();

    void SetVP(const Matrix4f& VP);

    void SetMinMaxHeight(float Min, float Max);

private:
    GLuint m_VPLoc = -1;
    GLuint m_minHeightLoc = -1;
    GLuint m_maxHeightLoc = -1;
    GLuint m_texHeight0UnitLoc = -1;
    GLuint m_texHeight1UnitLoc = -1;
    GLuint m_texHeight2UnitLoc = -1;
    GLuint m_texHeight3UnitLoc = -1;
};

#endif  /* TERRAIN_TECHNIQUE_H */
