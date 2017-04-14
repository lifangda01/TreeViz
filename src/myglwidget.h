#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "usda.h"
#include "camera.h"

#define M_WIDTH 800
#define M_HEIGHT 600
#define S_WIDTH 2048
#define S_HEIGHT 2048
#define C_TEXID 0   // Cubemap texture
#define S_TEXID 1   // Plane texture
#define D_TEXID 2   // Shadow map
#define M_TEXID 5   // Material textures

// Convert stencil value to branch index
#define ST2IDX(x) (x-2)
#define isbranch(x) (x>1 && x<thisManyBranches+2)

class myGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    myGLWidget(QWidget *parent = 0);
    ~myGLWidget();

    int showModel;
    int yRot;

    QString modelPath;
    QString modelName;
    ModelLoader *myModel;
    QString planeTex;
    QString texPath;
    QString cubePath;
    // Default material textures
    QString trunkTex;
    QString specTex;
    QString normTex;

    QVector<QVector3D> ppos;    // Point lights
    QVector3D ddir;             // Directional light

    void setupSelection(QVector<int> *sel, QVector<int> *del, int *curr, int *anal, int *hlgt);
    void setupNewModel(QString path, QString name);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

signals:
    void signal_updateInfo();
    void signal_updateAngle();

public slots:
    void on_cleanup();

private:
    void setupShaderProgram(QOpenGLShaderProgram *p, QString vshader, QString fShader);
    void setupBuffers();
    void setupVertexAttribs();
    void setupLighting();
    void setMaterialUniforms(MaterialInfo &mater);
    void setupTextures(Node *node, unsigned int texID);
    void setupCubemap();
    void setupShadow();
    void setupPlane();
    void setupStencil();
    void setupCuts();

    void renderShadow();
    void paintHelper();
    void drawScaledObject();
    void drawObject();
    void drawShadowNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix);
    void drawScaledNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix);
    void drawNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix);
    void drawCubemap();
    void drawPlane();
    void drawCuts();

    QVector<QVector<float> > XYZtoUV(QVector<float> vertices);
    void logMsgHandler(const QOpenGLDebugMessage &msg);
    void keyMovement();

    QOpenGLDebugLogger *logger;

    /* For main object */
    QOpenGLShaderProgram *m_program;
    QOpenGLFunctions *f;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_nbo;
    QOpenGLBuffer m_tbo;
    QOpenGLBuffer m_ibo;
    QOpenGLBuffer m_tanbo;
    QOpenGLBuffer m_bitbo;

    QSharedPointer<Node> m_rootNode;

    LightInfo m_lightInfo;
    MaterialInfo m_materialInfo;
    int texID;
    QOpenGLTexture *m_texture;
    QVector<QOpenGLTexture*> m_textures;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    /* For stencil outlining */
    QOpenGLShaderProgram *st_program;

    /* For cutting points */
    QOpenGLShaderProgram *ct_program;
    QOpenGLVertexArrayObject ct_vao;
    QOpenGLBuffer ct_vbo;
    QVector<float> ct_vertices;

    /* For cubemap texturing */
    QOpenGLShaderProgram *c_program;
    QOpenGLTexture *c_cubemap;
    QOpenGLVertexArrayObject c_vao;
    QOpenGLBuffer c_vbo;

    /* For shadow mapping */
    QOpenGLShaderProgram *s_program;
    QOpenGLFramebufferObject *s_fbo;
    QMatrix4x4 s_projection;
    QMatrix4x4 s_view;

    QMatrix4x4 s_lightSpaceMatrix;
    GLuint s_depthMapFBO;
    GLuint s_depthMap;

    /* For ground plane */
    QOpenGLShaderProgram *p_program;
    QOpenGLVertexArrayObject p_vao;
    QOpenGLBuffer p_vbo;
    QOpenGLBuffer p_nbo;
    QOpenGLBuffer p_tbo;
    QOpenGLTexture *p_texture;

    /* Misc. */
    QTime myTime;
    QVector<bool> keys;
    GLfloat lastX, lastY;
    float lastyRot;
    GLuint clickX, clickY;
    GLuint clickIndex;
    bool firstMouse;

    QVector<int> *m_selected, *m_deleted;
    int *m_current, *m_analyze, *m_highlight;

    Camera m_camera;
};

#endif // MYGLWIDGET_H
