#ifndef EXPORT_OBJ_H
#define EXPORT_OBJ_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#undef _CRT_SECURE_NO_WARNINGS

inline unsigned nullCount(void* p) { return p == 0 ? 0 : 1; }

char* exportObjToString
(
	unsigned numVerts, unsigned numTris,
	const float* vertices,
	const int* indices,
	const float* normals = 0,
	const float* uvCoords = 0
)
{
	unsigned maxSizeStimate =
	64 * numVerts *
	(
		1 +		// positions are mandatory
		nullCount((void*)normals) +
		nullCount((void*)uvCoords)
	)
	+ 64 * numTris
	+ 256;	// extra space for comments, blanks, etc

	char* str = (char*)malloc(maxSizeStimate);

	int j = 0;
	// positions
	j += sprintf(str, "# positions\n");
	for (int i = 0; i < numVerts; i++)
	{
		j += sprintf(str + j, "v %7.f %7.f %7.f\n",
			vertices[3 * i + 0], vertices[3 * i + 1], vertices[3 * i + 2]);
	}
	j += sprintf(str + j, "\n");
	
	// normals
	if (normals)
	{
		j += sprintf(str + j, "# normals\n");
		for (int i = 0; i < numVerts; i++)
		{
			j += sprintf(str + j, "vn %7.f %7.f %7.f\n",
				normals[3 * i + 0], normals[3 * i + 1], normals[3 * i + 2]);
		}
		j += sprintf(str + j, "\n");
	}

	// uv coords
	if (uvCoords)
	{
		j += sprintf(str + j, "# tex coords\n");
		for (int i = 0; i < numVerts; i++)
		{
			j += sprintf(str + j, "vt %7.f %7.f\n",
				uvCoords[2 * i + 0], uvCoords[2 * i + 1]);
		}
		j += sprintf(str + j, "\n");
	}

	// triangles
	const unsigned BUFFSIZE = 64;
	static char strVert[BUFFSIZE];
	static char strNormal[BUFFSIZE];
	static char strUv[BUFFSIZE];
	j += sprintf(str + j, "# triangles\n");
	for (int i = 0; i < numTris; i++)
	{
		j += sprintf(str + j, "f");
		for (int ii = 0; ii < 3; ii++)
		{
			unsigned ind = indices[3 * i + ii] + 1;
			strVert[0] = strNormal[0] = strUv[0] = 0;
			sprintf(strVert, "%d", ind);
			if (normals) sprintf(strNormal, "%d", ind);
			if (uvCoords) sprintf(strUv, "%d", ind);
			j += sprintf(str + j, " %s/%s/%s", strVert, strNormal, strUv);
		}
		j += sprintf(str + j, "\n");
	}

	return str;
}

void exportObjToFile(const char* fileName,
	unsigned numVerts, unsigned numTris,
	const float* vertices,
	const int* indices,
	const float* normals = 0,
	const float* uvCoords = 0
)
{
	char* str = exportObjToString(numVerts, numTris, vertices, indices, normals, uvCoords);
	FILE* file = fopen(fileName, "w+");
	if (!file)
	{
		printf("Error: can not open %s\n", fileName);
		return;
	}
	fputs(str, file);
	fclose(file);
}

#endif
