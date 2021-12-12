#include <windows.h>

#include <GL/glew.h>
#include "GL/glut.h"
//#include "GLFW/glfw3.h"

#include "error.h"
#include "buffer.h"
#include "shader.h"

#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp> //core stuffs
#include <glm/vec3.hpp> //glm:vec3
#include <glm/vec4.hpp> //glm::vec4
#include <glm/mat4x4.hpp> //glm::mat4
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <glm/ext/matrix_transform.hpp>//glm::translate,glm::rotate, glm::perspective etc

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#define h_s 800    
#define w_s 1200
GLuint ubuffer;
GLuint f_index;
static GLuint sphere_vao;
static GLuint cube_vao;
static GLuint patch_vao;
static GLuint sky_vao;
static GLuint f_pid;
static GLuint s_pid;
static GLuint sky_pid;
static GLuint R=60;
static GLuint depth;
static GLuint DIM=1024;
static GLuint fbo;
static GLint texSmile;
glm::mat4 proj;
glm::mat4 sproj;
glm::mat4 sview;
glm::mat4 view;
glm::vec3 peye;
static float viewer_pos[4] = { 1.0f, 4.0f, 6.0f, 1.0f };
static float light[4] = { 1.0f, 4.0f, 6.0f,1.0f}; // light pos in eye space

//Variaveis para controle de camera ArcBall
GLfloat dist1, dist2;
GLfloat X1, X2;
GLfloat Y1, Y2;
GLfloat Z1, Z2;
GLfloat x_0, y_0;
static glm::mat4 M(1.0f);
glm::vec3 u = { 0.0f,0.0f,0.0f };
glm::vec3 v = { 0.0f,0.0f,0.0f };

static float theta2 = 0.0f;

static void idle(void)
{
    theta2 += 0.035f;
    glutPostRedisplay();
}


struct Material
{
    glm::vec4 amb;
    glm::vec4 dif;
    glm::vec4 spe;
    float shi;
};
static Material mat = {
    glm::vec4(1.0f,2.0f,3.0f,1.0f) ,
    glm::vec4(1.0f,0.0f,0.0f,1.0f) ,
    glm::vec4(1.0f,1.0f,1.0f,1.0f) ,
    64
};


static void loadProjectionMatrix(GLuint pid) //deve ser chamado antes de loadar a matriz
{

    glUseProgram(pid); //rendering shaders
    //projection matrix
    glm::mat4 transl = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 svp = transl * scale * sproj * sview;
    GLint loc_svp = glGetUniformLocation(pid, "svp");
    glUniformMatrix4fv(loc_svp, 1, GL_FALSE, glm::value_ptr(svp));
}



static void loadMaterial(GLuint pid, GLuint index,GLuint bind,glm::vec4 ma, glm::vec4 md, glm::vec4 ms) //deve ser chamado antes de loadar a matriz
{

    GLint loc_ma = glGetUniformLocation(f_pid, "ma");
    GLint loc_md = glGetUniformLocation(f_pid, "md");
    GLint loc_ms = glGetUniformLocation(f_pid, "ms");

    glUniformBlockBinding(pid, index, bind);

    glUseProgram(pid);
    glUniform4fv(loc_ma, 1, glm::value_ptr(ma));
    glUniform4fv(loc_md, 1, glm::value_ptr(md));
    glUniform4fv(loc_ms, 1, glm::value_ptr(ms));
}

static void loadMatrices(GLuint pid,glm::mat4 model)
{
     glm::mat4 mv =  view * model;
     glm::mat4 mvp = proj * mv;
     //glm::mat4 inv = glm::inverse(mv);
     //glm::mat4 nm = glm::transpose(glm::inverse(mv));


     //GLint loc_mv = glGetUniformLocation(pid, "mv");
     GLint loc_mvp = glGetUniformLocation(pid, "mvp");
     //GLint loc_inv = glGetUniformLocation(pid, "inv");
     //GLint loc_nm = glGetUniformLocation(pid, "nm");
     GLint loc_model = glGetUniformLocation(pid, "model");
     GLint loc_view = glGetUniformLocation(pid, "view");
     GLint loc_proj = glGetUniformLocation(pid, "proj");

     glUseProgram(pid);
     //glUniformMatrix4fv(loc_mv, 1, GL_FALSE, glm::value_ptr(mv));
     glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
     //glUniformMatrix4fv(loc_inv, 1, GL_FALSE, glm::value_ptr(inv));
     //glUniformMatrix4fv(loc_nm, 1, GL_FALSE, glm::value_ptr(nm));
     glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
     glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
     glUniformMatrix4fv(loc_proj, 1, GL_FALSE, glm::value_ptr(proj));
     
}

/*
static void loadShadowMatrices(GLuint pid, glm::mat4 model)
{
    glm::mat4 mv = view * model;
    //glm::mat4 mvp = proj * mv;
    //glm::mat4 inv = glm::inverse(mv);
    //glm::mat4 nm = glm::transpose(glm::inverse(mv));


    //GLint loc_mv = glGetUniformLocation(pid, "mv");
    //GLint loc_mvp = glGetUniformLocation(pid, "mvp");
    //GLint loc_inv = glGetUniformLocation(pid, "inv");
    //GLint loc_nm = glGetUniformLocation(pid, "nm");
    GLint loc_model = glGetUniformLocation(pid, "model");
    GLint loc_view = glGetUniformLocation(pid, "view");
    GLint loc_proj = glGetUniformLocation(pid, "proj");

    glUseProgram(pid);
    //glUniformMatrix4fv(loc_mv, 1, GL_FALSE, glm::value_ptr(mv));
    //glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    //glUniformMatrix4fv(loc_inv, 1, GL_FALSE, glm::value_ptr(inv));
    //glUniformMatrix4fv(loc_nm, 1, GL_FALSE, glm::value_ptr(nm));
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(sview));
    glUniformMatrix4fv(loc_proj, 1, GL_FALSE, glm::value_ptr(sproj));

}
*/
/*
static void loadMatricesSky(GLuint pid)
{
    GLint skyloc_sky = glGetUniformLocation(sky_pid, "sky");
    GLint skyloc_mvp = glGetUniformLocation(sky_pid, "mvp");
    glm::mat4 sky_model = glm::translate(glm::mat4(1.0f), glm::vec3(peye));
    glm::mat4 mv = view * sky_model;
    glm::mat4 mvp = proj * mv;
    glUseProgram(sky_pid);
    glUniform1i(skyloc_sky, 0);
    glUniformMatrix4fv(skyloc_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

}
*/
unsigned int CreateDepthTexture(int width, int height)
{
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT, width,height,0,
        GL_DEPTH_COMPONENT,GL_FLOAT,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
        GL_COMPARE_REF_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D,0);
    return tex;
}


GLuint CreateTexture2D(const char * filename)
{
    GLuint tex;
    int width, height, nchannels;
    unsigned char *data = stbi_load(filename,&width,&height,&nchannels,3);
    if (!data) return 0;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

static unsigned char* extract_subimage(int W, int n, unsigned char* data,int x, int y, int w, int h)
{
    unsigned char*img =(unsigned char*)malloc(w*h*n);
    for(int i = 0; i < h; ++i) {
        memcpy(img+(i*w)*n, data +((y+i)*W+x)*n,w*n);
    }
    return img;
}

unsigned char CreateSkyBox(const char* filename)
{
    int width, height, nchannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nchannels, 0);
    if (!data) {
        fprintf(stderr, "Could not load image:%s\n", filename);
        exit(1);
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    //subimages’ dimension
    int w = width/4, h = height/3;
    int x[] = {2*w, 0, w, w, w,3*w};
    int y[] = { h, h, 0,2*h, h, h};
    GLenum face[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // top
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // bottom
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // front
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // back
    };
    for(int i = 0; i < 6; ++i) {
        unsigned char * img = extract_subimage(width,nchannels,data,x[i],y[i],w,h);
        glTexImage2D(face[i],0,GL_RGB,w,h,0,nchannels==3?GL_RGB:GL_RGBA,GL_UNSIGNED_BYTE,img);
        free(img);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    stbi_image_free(data);
    return tex;
}


static void getCoord(float s, float t, float* x, float* y, float* z)
{
    float theta = s * 2 * M_PI;
    float phi = t * M_PI;
    *x = sin(theta) * sin(phi);
    *y = cos(phi);
    *z = cos(theta) * sin(phi);
}

// allocate and fill the coordinate array of dimension 3*(nx+1)*(ny+1)
static float* sphereCoords(int nx, int ny, float** texcoords, float** tangent)
{
    float* coords = (float*)malloc(3 * (nx + 1) * (ny + 1) * sizeof(float));
    (*texcoords) = (float*)malloc(2 * (nx + 1) * (ny + 1) * sizeof(float));
    (*tangent) = (float*)malloc(3 * (nx + 1) * (ny + 1) * sizeof(float));
    float dx = 1.0f / nx;
    float dy = 1.0f / ny;
    int n = 0;
    int t = 0;
    int tan = 0;
    for (int j = 0; j <= ny; ++j) {
        for (int i = 0; i <= nx; ++i) {
            float x, y, z;
            getCoord(i * dx, j * dy, &x, &y, &z);
            coords[n++] = x;
            coords[n++] = y;
            coords[n++] = z;
            (*texcoords)[t++] = i * dx;
            (*texcoords)[t++] = j * dy;
            (*tangent)[tan++] = (2 * M_PI * i * cos(2 * M_PI * i * dx) * sin(M_PI * j * dy));
            (*tangent)[tan++] = 0;
            (*tangent)[tan++] = (-2 * M_PI * i * sin(2 * M_PI * i * dx) * sin(M_PI * j * dy));
        }
    }
    return coords;
}

#define INDEX(i,j,nx) ((j)*(nx)+(i))

// allocate and fill the incidence array of dimension 6*nx*ny
static unsigned int* sphereIncidence(int nx, int ny)
{
    unsigned int* inc = (unsigned int*)malloc((6 * nx * ny) * sizeof(unsigned int));
    int n = 0;
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            inc[n++] = INDEX(i, j, nx);
            inc[n++] = INDEX(i + 1, j + 1, nx);
            inc[n++] = INDEX(i + 1, j, nx);
            inc[n++] = INDEX(i, j, nx);
            inc[n++] = INDEX(i, j + 1, nx);
            inc[n++] = INDEX(i + 1, j + 1, nx);
        }
    }
    return inc;
}

static GLuint createSphere(int nx, int ny)
{
    int csize = 3 * (nx + 1) * (ny + 1);
    int tsize = 2 * (nx + 1) * (ny + 1);
    float* texcoord;
    float* tangent;
    float* coord = sphereCoords(nx, ny, &texcoord,&tangent);
    int isize = 6 * nx * ny;
    unsigned int* index = sphereIncidence(nx, ny);
    GLuint vao = CreateVAO();
    CreateBuffer(GL_ARRAY_BUFFER, csize * sizeof(float), (GLvoid*)coord);
    VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CreateBuffer(GL_ARRAY_BUFFER, tsize * sizeof(float), (GLvoid*)texcoord);
    VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    CreateBuffer(GL_ARRAY_BUFFER, csize * sizeof(float), (GLvoid*)tangent);
    VertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(unsigned int),(GLvoid*)index);
    free(index);
    free(coord);
    free(texcoord);
    free(tangent);
    return vao;
}

/*
glm::vec3 * cubeNormals(GLfloat* cube_vertices)
{
    GLint index = 0;
    glm::vec3* normalMap = (glm::vec3*)malloc(3 * (0.5 + 1) * (0.5 + 1) * sizeof(glm::vec3));
    while (cube_vertices[index + 2] != NULL)
    {
        glm::vec3 normal = normalize(cross(glm::vec3(cube_vertices[index + 2] - cube_vertices[index + 2]), glm::vec3(cube_vertices[index] - cube_vertices[index + 1])));
        normalMap[index] = normal;
        index++;
    }
    return normalMap;
}
*/

static GLuint createCube(GLfloat halfSideLength)
{
    GLfloat cube_vertices[] =
    {
        // front face
        0 - halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // top left
        0 + halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // top right
        0 + halfSideLength, 0 - halfSideLength, 0 + halfSideLength, // bottom right
        0 - halfSideLength, 0 - halfSideLength, 0 + halfSideLength, // bottom left

        // back face
        0 - halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // top left
        0 + halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // top right
        0 + halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // bottom right
        0 - halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // bottom left

        // left face
        0 - halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // top left
        0 - halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // top right
        0 - halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // bottom right
        0 - halfSideLength, 0 - halfSideLength, 0 + halfSideLength, // bottom left

        // right face
        0 + halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // top left
        0 + halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // top right
        0 + halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // bottom right
        0 + halfSideLength, 0 - halfSideLength, 0 + halfSideLength, // bottom left

        // top face
        0 - halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // top left
        0 - halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // top right
        0 + halfSideLength, 0 + halfSideLength, 0 - halfSideLength, // bottom right
        0 + halfSideLength, 0 + halfSideLength, 0 + halfSideLength, // bottom left

        // top face
        0 - halfSideLength, 0 - halfSideLength, 0 + halfSideLength, // top left
        0 - halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // top right
        0 + halfSideLength, 0 - halfSideLength, 0 - halfSideLength, // bottom right
        0 + halfSideLength, 0 - halfSideLength, 0 + halfSideLength  // bottom left   
    };

    GLuint index[] = {
    0,1,2,0,2,3,4,5,6,4,6,7,8,9,10,8,10,11,
    12,13,14,12,14,15,16,17,18,16,18,19,20,21,22,20,22,23
    };

    GLuint vao = CreateVAO();
    CreateBuffer(GL_ARRAY_BUFFER, sizeof(cube_vertices), (GLvoid*)cube_vertices);
    VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //GLfloat *normal=cubeNormals(cube_vertices);
    //CreateBuffer(GL_ARRAY_BUFFER, sizeof(cube_normals), (GLvoid*)cube_normals);
    VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //CreateBuffer(GL_ARRAY_BUFFER, sizeof(texcoord), (GLvoid*)texcoord);
    //VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //CreateBuffer(GL_ARRAY_BUFFER, sizeof(texcoord), (GLvoid*)tangent);
    //VertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), (GLvoid*)index);

    return vao;
}

/*
static GLuint createSkyBox(void)
{
    float v[] = {
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
    };
    GLuint vao = CreateVAO();
    CreateBuffer(GL_ARRAY_BUFFER, sizeof(v), (GLvoid*)v);
    VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    return vao;
}

static GLuint createPatch(void)//Necessario para tesselation shader
{
    float geom[] = {
    0.0f,0.0f,0.0f,2.0f
    };
    glPatchParameteri(GL_PATCH_VERTICES,1);
    GLuint vao = CreateVAO();
    CreateBuffer(GL_ARRAY_BUFFER,sizeof(geom),(GLvoid*)geom);
    VertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
    return vao;
}
*/
static void createProjScene(void) 
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glDrawBuffer(GL_NONE); //sem buffer de cor
    glViewport(0, 0, DIM, DIM); //shadow map dimension
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(s_pid); //shaders para gerar mapa de sombra
    //projection matrix
    loadProjectionMatrix(f_pid);
    //bind texture
    GLint loc_sampler = glGetUniformLocation(f_pid, "smile");
    glUniform1i(loc_sampler, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth);


    glPolygonOffset(2.0f,2.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);


    //-------------------------Criando esfera1-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(0.0f, 1.85f, 0.0f));
    sphere_model = glm::scale(sphere_model, glm::vec3(1.5f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
    /*glm::vec4 maEsfera(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdEsfera(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 msEsfera(0.5f, 0.5f, 0.5f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera, mdEsfera, msEsfera);*/
    loadMatrices(s_pid, sphere_model);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);

    //-------------------------Criando esfera2-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model2 = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(2.3f, 1.35f, 0.0f));
    sphere_model2 = glm::scale(sphere_model2, glm::vec3(1.0f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
    /*glm::vec4 maEsfera2(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdEsfera2(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 msEsfera2(0.5f, 0.5f, 0.5f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera2, mdEsfera2, msEsfera2);*/
    loadMatrices(s_pid, sphere_model2);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);

    //-------------------------Criando esfera3-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model3 = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(-2.3f, 1.35f, 0.0f));
    sphere_model3 = glm::scale(sphere_model3, glm::vec3(1.0f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
   /*glm::vec4 maEsfera3(0.5f, 0.8f, 0.2f, 1.0f);
    glm::vec4 mdEsfera3(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 msEsfera3(0.5f, 0.5f, 0.5f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera3, mdEsfera3, msEsfera3);*/
    loadMatrices(s_pid, sphere_model3);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);

    //-------------------------Criando Mesa-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 cube_model = glm::translate(glm::mat4(1.0),
        glm::vec3(0.0f, 0.0f, 0.0f));
    cube_model = glm::scale(cube_model, glm::vec3(6.5f, 0.7f, 6.5f));
    /*glm::vec4 maMesa(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 mdMesa(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 msMesa(0.0f, 0.0f, 1.0f, 1.0f);

    loadMaterial(f_pid, f_index, 2, maMesa, mdMesa, msMesa);*/
    loadMatrices(s_pid, cube_model);

    glBindVertexArray(cube_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

static void createScene(void)
{   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, w_s, h_s); //window dimension
    glClearColor(0.8f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec4 leye = glm::vec4(-peye[0], -peye[1], -peye[2], 1.0f);
    GLint f_loc_leye = glGetUniformLocation(f_pid, "leye");
    glUseProgram(f_pid);
    glUniform4fv(f_loc_leye, 1, glm::value_ptr(leye));

    GLint loc_sampler = glGetUniformLocation(f_pid, "smile");
    glUniform1i(loc_sampler, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texSmile);
    //-------------------------Criando esfera1-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(0.0f,1.85f,0.0f));
    sphere_model = glm::scale(sphere_model, glm::vec3(1.5f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
    glm::vec4 maEsfera (0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdEsfera (1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 msEsfera (0.0f, 0.0f, 1.0f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera, mdEsfera, msEsfera);
    loadProjectionMatrix(f_pid);
    loadMatrices(f_pid, sphere_model);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);


    //-------------------------Criando esfera2-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model2 = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(2.3f, 1.35f, 0.0f));
    sphere_model2 = glm::scale(sphere_model2, glm::vec3(1.0f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
    glm::vec4 maEsfera2(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdEsfera2(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 msEsfera2(0.5f, 0.5f, 0.5f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera2, mdEsfera2, msEsfera2);
    loadMatrices(f_pid, sphere_model2);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);

    //-------------------------Criando esfera3-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 sphere_model3 = glm::translate(glm::mat4(1.0f), // Altera posicao do desenho 3D
        glm::vec3(-2.3f, 1.35f, 0.0f));
    sphere_model3 = glm::scale(sphere_model3, glm::vec3(1.0f)); // altera escala do desenho
    //f_model = glm::rotate(f_model, -theta2, glm::vec3(0.0f,1.0f,0.0f)); //Rotaciona a esfera via func idle
    glm::vec4 maEsfera3(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdEsfera3(0.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 msEsfera3(0.5f, 0.5f, 0.5f, 1.0f);
    loadMaterial(f_pid, f_index, 2, maEsfera3, mdEsfera3, msEsfera3);
    loadMatrices(f_pid, sphere_model3);
    glBindVertexArray(sphere_vao);
    glDrawElements(GL_TRIANGLES, 6 * R * R, GL_UNSIGNED_INT, 0);

    //-------------------------Criando Mesa-------------------------------//
    //-----------------------------------------------------------------------//
    glm::mat4 cube_model = glm::translate(glm::mat4(1.0),
        glm::vec3(0.0f, 0.0f, 0.0f));
    cube_model = glm::scale(cube_model, glm::vec3(6.5f, 0.7f, 6.5f));
    glm::vec4 maMesa(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 mdMesa(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 msMesa(0.5f, 0.5f, 0.5f, 1.0f);

    loadMaterial(f_pid, f_index, 2, maMesa, mdMesa, msMesa);
    loadMatrices(f_pid, cube_model);

    glBindVertexArray(cube_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

static void setProgram ()
{
    GLuint vid = CreateShader(GL_VERTEX_SHADER, "vert.glsl");
    GLuint s_vid = CreateShader(GL_VERTEX_SHADER, "vertShadow.glsl");
    GLuint fid = CreateShader(GL_FRAGMENT_SHADER, "frag.glsl");
    GLuint s_fid = CreateShader(GL_FRAGMENT_SHADER, "fragShadow.glsl");
    f_pid = CreateProgram(vid,fid,0);
    s_pid = CreateProgram(s_vid, s_fid, 0);
}


static void setCamera()
{
    int window[4]; // window dimension:{x0,y0,w,h}
    glGetIntegerv(GL_VIEWPORT, window);
    float ratio = (float)window[2] / window[3];
    proj = glm::perspective(
        glm::radians(70.0f), ratio, 1.0f, 10.0f
    );
    view = M * glm::lookAt(
        glm::vec3(viewer_pos[0], viewer_pos[1], viewer_pos[2]), // eye
        glm::vec3(0.0f, 0.0f, 0.0f), // center
        glm::vec3(0.0f, 1.0f, 0.0f) // up
    );
    glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    peye = glm::vec3(glm::inverse(view) * origin);

    //GLint f_loc_leye = glGetUniformLocation(f_pid, "leye");
    //glUseProgram(f_pid);
    //glUniform4fv(f_loc_leye, 1, glm::value_ptr(peye));

    //light "camera"
    sproj = glm::perspective(
        glm::radians(35.0f), 1.0f, 1.0f, 10.0f
    );
    sview = glm::lookAt(
        glm::vec3(light[0], light[1], light[2]),//lightpos
        glm::vec3(0.0f, 0.0f, 0.0f),//center
        glm::vec3(0.0f, 1.0f, 0.0f)//up
    );
    createProjScene();
}

static void initialize()
{
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        exit(-1);
    }
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    glClearColor(1.0f,1.0f,1.0f,1.0f);          //Inicializa com a cor de tela branca
    glEnable(GL_DEPTH_TEST);
    texSmile = CreateTexture2D("smile.png");
    sphere_vao = createSphere(R,R);             // Cria vao de esfera
    cube_vao = createCube(0.5f); // Cria Vao de cubo no centro
    //sky_vao = createSkyBox();
    //patch_vao = createPatch();
    setProgram();                               // Cria f_pid
    //create depth texture
    depth = CreateDepthTexture(DIM,DIM);
    //----------------------create fbo-------------------------------//
    //----------------------------------------------------------------------//
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, depth,0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        printf("Framebuffer not complete\n");
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //----------------------Definindo material-------------------------------//
    //----------------------------------------------------------------------//
    ubuffer = CreateBuffer(GL_UNIFORM_BUFFER, sizeof(Material), (GLvoid*)&mat);
    f_index = glGetUniformBlockIndex(f_pid, "Material");
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubuffer);

}

static void display(void)//GLFWwindow * win
{
    setCamera();
    createScene();
    glutSwapBuffers();

}

// Reshape callback
static void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}


// Keyboard callback
static void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: exit(0); break;
    }
}

static void motionCB(int x, int y) {
    float ax, ay, az;
    glm::vec3 a;
    //float d = viewer_pos[2];
    float r = fmin(w_s, h_s) / 2;
    // x1, y1 --> X1, Y1, Z1
    X1 = (float(x_0) - (w_s / 2)) / r;
    Y1 = (float(y_0) - (h_s / 2)) / r;
    dist1 = sqrt(((X1 * X1 + Y1 * Y1)));
    // x, (h - y) --> X2, Y2, Z2
    y = h_s - y;
    X2 = ((float)x - (w_s / 2)) / r;
    Y2 = ((float)y - (h_s / 2)) / r;
    dist2 = sqrtf(X2 * X2 + Y2 * Y2);
    if (dist1 > 1)
    {
        X1 = X1 / dist1;
        Y1 = Y1 / dist1;
        Z1 = 0.0f;
    }
    if (dist1 <= 1)
    {
        dist1 = sqrt(X1 * X1 + Y1 * Y1);
        dist1 = (1 - (dist1 * dist1));
        Z1 = sqrt(dist1);
    }

    if (dist2 > 1)
    {
        X2 = X2 / dist2;
        Y2 = Y2 / dist2;
        Z2 = 0.0f;
    }
    if (dist2 <= 1)
    {

        dist2 = sqrt(X2 * X2 + Y2 * Y2);
        dist2 = (1 - (dist2 * dist2));
        Z2 = sqrtf(dist2);

    }

    // u, v --> a, theta
    u[0] = X1;
    u[1] = Y1;
    u[2] = Z1;

    v[0] = X2;
    v[1] = Y2;
    v[2] = Z2;
    //vet_product(u, v, a);
    a = cross(glm::vec3(u), glm::vec3(v));
    ax = a[0];
    ay = a[1];
    az = a[2];
    float theta = 2 * asin(sqrtf(powf(ax, 2) + powf(ay, 2) + powf(az, 2)));
    float d = sqrt(
        (viewer_pos[0] * viewer_pos[0]) +
        (viewer_pos[1] * viewer_pos[1]) +
        (viewer_pos[2] * viewer_pos[2]));
    /*printf("coords u:%f,%f,%f\n", X1, Y1, Z1);
    printf("coords v:%f,%f,%f\n", X2, Y2, Z2);
    printf("coords a:%f,%f,%f\n", ax, ay, az);
    printf("THETA   :%f \n", theta);*/

    glPushMatrix();
    glm::mat4 aux(1.0f);
    aux = glm::translate(aux, glm::vec3(0.0f, 0.0f, -d));
    if (theta != 0)
    {
        aux = glm::rotate(aux, theta, glm::vec3(ax, ay, az));
    }
    aux = glm::translate(aux,glm::vec3(0.0f, 0.0f, d));
    M = aux * M; 
    glPopMatrix();

    x_0 = (float)x;
    y_0 = (float)y;

    glutPostRedisplay();
}

static void mouseCB(int button, int state, int x, int y) {
    float r = fmin(w_s, h_s) / 2;
    if (button != GLUT_LEFT_BUTTON)
    {
        return;
    }
    else if (state == GLUT_DOWN)
    {
        x_0 = (float)x;
        y_0 = h_s - float(y);
        glutMotionFunc(motionCB);
    }
    else {
        glutMotionFunc(NULL);
    }
}

// Main function
int main(int argc, char* argv[])
{   
    // open GLUT 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(w_s, h_s);
    // create window
    glutCreateWindow("Core");
    glutReshapeFunc(reshape);
    initialize();
    glutDisplayFunc(display);
    // interact... 
    glutMouseFunc(mouseCB);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}