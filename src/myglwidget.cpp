#include "myglwidget.h"

GLfloat skyboxVertices[] = {
    // Positions
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

GLfloat planeVertices[] = {
    // Positions
    25.0f, -0.5f, 25.0f,
    -25.0f, -0.5f, -25.0f,
    -25.0f, -0.5f, 25.0f,

    25.0f, -0.5f, 25.0f,
    25.0f, -0.5f, -25.0f,
    -25.0f, -0.5f, -25.0f
};

GLfloat planeNorms[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

GLfloat planeTexCoords[] = {
    10.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f,
    10.0f, 0.0f, 10.0f, 10.0f, 0.0f, 10.0f
};

GLfloat quadVertices[] = {
    // Positions
    -0.05f,  0.05f, 0.0f,
    -0.05f, -0.05f, 0.0f,
    0.05f, -0.05f, 0.0f,
    0.05f, -0.05f, 0.0f,
    0.05f,  0.05f, 0.0f,
    -0.05f,  0.05f, 0.0f
};

myGLWidget::myGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    // OpenGL 3.3 debug context using the Core profile
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    //    format.setSamples(4);
    setFormat(format);

    myTime = QTime();
    myTime.start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);

    setFocusPolicy(Qt::ClickFocus);

    //    modelPath = QString("/home/fangda/Datasets/Trees/tree1/");
    //    modelName = QString("cluster_14_mesh.ply");
    //    modelPath = QString("/home/fangda/Datasets/Trees/tree1/");
    //    modelName = QString("testtree.obj");
    //    modelPath = QString("/home/fangda/Datasets/nanosuit/");
    //    modelName = QString("nanosuit.obj");

    texPath = QString("./resources/textures/");
    planeTex = texPath + QString("grid16.png");
    trunkTex = texPath + QString("branchTex.png");
    specTex = texPath + QString("white_spec.png");
    normTex = texPath + QString("testnorm1.png");
    cubePath = QString("./resources/textures/");
}

myGLWidget::~myGLWidget()
{
    on_cleanup();
}

void myGLWidget::on_cleanup()
{
    makeCurrent();
    if (m_vao.isCreated()) m_vao.destroy();
    doneCurrent();
}

void myGLWidget::setupNewModel(QString path, QString name)
{
    modelPath = path;
    modelName = name;
    makeCurrent();
    setupBuffers();
    doneCurrent();
}

void myGLWidget::setupShaderProgram(QOpenGLShaderProgram *p, QString vShader, QString fShader)
{
    // Setup OpenGL context and bind shaders
    p->addShaderFromSourceFile(QOpenGLShader::Vertex, vShader.toUtf8());
    p->addShaderFromSourceFile(QOpenGLShader::Fragment, fShader.toUtf8());
    if ((!p->link()) || (!p->bind()))
    {
        qCritical() << "Could not bind shaders";
        QMessageBox::critical(this, tr("No Shader Found"),
                              tr("Cannot bind shaders."),
                              QMessageBox::Ok);
    }
}

void myGLWidget::setupBuffers()
{
    // Model must be a class instance
    myModel = &model;

    if (myModel->isEmpty() &&
            !myModel->Load(modelPath + modelName, ModelLoader::AbsolutePath))
    {
        qCritical() << "Could not load model";
        QMessageBox::critical(this, tr("No Mesh Found"),
                              tr("Cannot find tree mesh file."),
                              QMessageBox::Ok);
        return;
    }

    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<QVector<float> > *textureUV; // Channels * UVs
    QVector<float> *tangents;
    QVector<float> *bitangents;
    QVector<unsigned int> *indices;

    myModel->getBufferData(&vertices, &normals, &indices);
    myModel->getTextureData(&textureUV, &tangents, &bitangents);

    // VAO has four buffers: vertex, normal, index, uv (optional)
    m_vao.create();
    m_vao.bind();

    // Vertex
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(&(*vertices)[0], vertices->size() * sizeof(float));
    // Normal
    m_nbo.create();
    m_nbo.bind();
    m_nbo.allocate(&(*normals)[0], normals->size() * sizeof(float));
    // Index
    m_ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(&(*indices)[0], indices->size() * sizeof(unsigned int));
    // UV
    // If no UV coordinate is provided, generate UV based on cylindrical model
    if (textureUV->isEmpty())
    {
        *textureUV = XYZtoUV(*vertices);
        qDebug() << "UV empty, XYZ to UV!!";
    }
    m_tbo.create();
    m_tbo.bind();
    int texSize = 0;
    for(int ii=0; ii<textureUV->size(); ++ii)
        texSize += textureUV->at(ii).size();

    m_tbo.allocate(&(*textureUV)[0][0], texSize * sizeof(float));

    //    // Normal Mapping
    //    if (tangents->isEmpty() || bitangents->isEmpty())
    //    {
    //        // Generate using index here
    //        tangents->resize(vertices->size());
    //        bitangents->resize(vertices->size());
    //        for (uint i = 0; i < indices->size(); i+=3)
    //        {
    //            // Positions
    //            QVector3D pos1(vertices->at(3*indices->at(i)),   //x
    //                           vertices->at(3*indices->at(i)+1), //y
    //                           vertices->at(3*indices->at(i)+2));//z
    //            QVector3D pos2(vertices->at(3*indices->at(i+1)),
    //                           vertices->at(3*indices->at(i+1)+1),
    //                           vertices->at(3*indices->at(i+1)+2));
    //            QVector3D pos3(vertices->at(3*indices->at(i+2)),
    //                           vertices->at(3*indices->at(i+2)+1),
    //                           vertices->at(3*indices->at(i+2)+2));
    //            // Texture coordinates
    //            QVector<float> tempUV;
    //            tempUV = (*textureUV)[0];
    //            QVector2D uv1(tempUV[2*indices->at(i)],   //u
    //                          tempUV[2*indices->at(i)+1]);//v
    //            QVector2D uv2(tempUV[2*indices->at(i+1)],
    //                          tempUV[2*indices->at(i+1)+1]);
    //            QVector2D uv3(tempUV[2*indices->at(i+2)],
    //                          tempUV[2*indices->at(i+2)+1]);

    //            QVector3D edge1 = pos2 - pos1;
    //            QVector3D edge2 = pos3 - pos1;
    //            QVector2D deltaUV1 = uv2 - uv1;
    //            QVector2D deltaUV2 = uv3 - uv1;

    //            GLfloat f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    //            QVector3D tan, bit;

    //            tan.setX( f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()) );
    //            tan.setZ( f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()) );
    //            tan.setY( f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()) );
    //            tan.normalize();

    //            bit.setX( f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()) );
    //            bit.setZ( f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()) );
    //            bit.setY( f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()) );
    //            bit.normalize();

    //            (*tangents)[3*indices->at(i)]   = tan.x();
    //            (*tangents)[3*indices->at(i)+1] = tan.y();
    //            (*tangents)[3*indices->at(i)+2] = tan.z();

    //            (*tangents)[3*indices->at(i+1)]   = tan.x();
    //            (*tangents)[3*indices->at(i+1)+1] = tan.y();
    //            (*tangents)[3*indices->at(i+1)+2] = tan.z();

    //            (*tangents)[3*indices->at(i+2)]   = tan.x();
    //            (*tangents)[3*indices->at(i+2)+1] = tan.y();
    //            (*tangents)[3*indices->at(i+2)+2] = tan.z();

    //            (*bitangents)[3*indices->at(i)]   = bit.x();
    //            (*bitangents)[3*indices->at(i)+1] = bit.y();
    //            (*bitangents)[3*indices->at(i)+2] = bit.z();

    //            (*bitangents)[3*indices->at(i+1)]   = bit.x();
    //            (*bitangents)[3*indices->at(i+1)+1] = bit.y();
    //            (*bitangents)[3*indices->at(i+1)+2] = bit.z();

    //            (*bitangents)[3*indices->at(i+2)]   = bit.x();
    //            (*bitangents)[3*indices->at(i+2)+1] = bit.y();
    //            (*bitangents)[3*indices->at(i+2)+2] = bit.z();
    ////            qDebug() << indices->at(i) << indices->at(i+1) << indices->at(i+2);
    ////            tangents->push_back(tan.x());
    ////            tangents->push_back(tan.y());
    ////            tangents->push_back(tan.z());

    ////            bitangents->push_back(bit.x());
    ////            bitangents->push_back(bit.y());
    ////            bitangents->push_back(bit.z());
    //        }
    //    }
    //    m_tanbo.create();
    //    m_bitbo.create();

    //    m_tanbo.bind();
    //    m_tanbo.allocate(&(*tangents)[0], tangents->size() * sizeof(float));

    //    m_bitbo.bind();
    //    m_bitbo.allocate(&(*bitangents)[0], bitangents->size() * sizeof(float));
    //    qDebug() << "normal mapping!";

    m_rootNode = myModel->getNodeData();
}

void myGLWidget::setupPlane()
{
    p_program = new QOpenGLShaderProgram;
    setupShaderProgram(p_program, "src/plane.vert", "src/plane.frag");

    p_program->bind();

    // Dont generate mipmap to protect alpha value
    p_texture = new QOpenGLTexture(QImage(planeTex).mirrored(), QOpenGLTexture::DontGenerateMipMaps);
    if (QImage(planeTex).isNull()){
        qWarning() << "Could not load plane texture.";
        QMessageBox::critical(this, tr("No Plane Texture Found"),
                              tr("Cannot find plane texture image."),
                              QMessageBox::Ok);
    }

    p_vao.create();
    p_vao.bind();

    p_vbo.create();
    p_vbo.bind();
    p_vbo.allocate(planeVertices, 6*3*sizeof(GLfloat));

    p_nbo.create();
    p_nbo.bind();
    p_nbo.allocate(planeNorms, 6*3*sizeof(GLfloat));

    p_tbo.create();
    p_tbo.bind();
    p_tbo.allocate(planeTexCoords, 6*2*sizeof(GLfloat));

    p_vbo.bind();
    p_program->enableAttributeArray( 0 );      // layout location
    p_program->setAttributeBuffer( 0,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)

    p_nbo.bind();
    p_program->enableAttributeArray( 1 );      // layout location
    p_program->setAttributeBuffer( 1,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)

    p_tbo.bind();
    p_program->enableAttributeArray( 2 );      // layout location
    p_program->setAttributeBuffer( 2,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   2);         // number of components (3 for x,y,z)

    p_vao.release();
}

void myGLWidget::setupVertexAttribs()
{
    m_program->bind();
    m_vao.bind();

    m_vbo.bind();
    m_program->enableAttributeArray( 0 );      // layout location
    m_program->setAttributeBuffer( 0,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)

    m_nbo.bind();
    m_program->enableAttributeArray( 1 );      // layout location
    m_program->setAttributeBuffer( 1,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)

    if (m_tbo.isCreated())
    {
        m_tbo.bind();
        m_program->enableAttributeArray( 2 );      // layout location
        m_program->setAttributeBuffer( 2,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       2);         // number of components (2 for u,v)
    }

    if (m_tanbo.isCreated() && m_bitbo.isCreated())
    {
        m_tanbo.bind();
        m_program->enableAttributeArray( 3 );
        m_program->setAttributeBuffer( 3,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)

        m_bitbo.bind();
        m_program->enableAttributeArray( 4 );
        m_program->setAttributeBuffer( 4,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)
    }

}

void myGLWidget::setupLighting()
{
    // Point lights, there is a better way to do this in adv glsl
    ppos.append(QVector3D(0.7f,  3.0f,  2.0f));
    ppos.append(QVector3D(2.3f, -3.3f, -4.0f));
    ppos.append(QVector3D(-4.0f,  2.0f, -12.0f));
    ppos.append(QVector3D(0.0f,  3.0f, -3.0f));

    for (int i = 0; i < 4; i++)
    {
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].position"),
                                   ppos[i]);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].color"),
                                   QVector3D(1.0f, 1.0f, 1.0f));
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].ambient"),
                                   0.5f);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].diffuse"),
                                   1.0f);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].specular"),
                                   1.0f);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].constant"),
                                   1.0f);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].linear"),
                                   0.09f);
        m_program->setUniformValue(m_program->uniformLocation("pointLights["+QString::number(i)+"].quadratic"),
                                   0.032f);
    }

    // Directional light
    ddir = QVector3D(-1.0f, -1.0f, 1.0f);

    m_program->setUniformValue(m_program->uniformLocation("dirLight.direction"),
                               ddir);
    m_program->setUniformValue(m_program->uniformLocation("dirLight.color"),
                               QVector3D(1.0f, 1.0f, 1.0f));
    m_program->setUniformValue(m_program->uniformLocation("dirLight.ambient"),
                               0.5f);
    m_program->setUniformValue(m_program->uniformLocation("dirLight.diffuse"),
                               0.5f);
    m_program->setUniformValue(m_program->uniformLocation("dirLight.specular"),
                               0.5f);
}

void myGLWidget::setMaterialUniforms(MaterialInfo &mater)
{
    // shininess 0 has awesome cartoon effect: specular either on or totally off
    m_program->setUniformValue( "shininess", mater.Shininess );

    f->glActiveTexture(GL_TEXTURE0 + M_TEXID + mater.diffTexID);
    m_textures[mater.diffTexID]->bind();
    m_program->setUniformValue("diffuseTexture", M_TEXID + mater.diffTexID);

    if (!mater.specTexName.isEmpty())
    {
        f->glActiveTexture(GL_TEXTURE0 + M_TEXID + mater.specTexID);
        m_textures[mater.specTexID]->bind();
        m_program->setUniformValue("specularTexture", M_TEXID + mater.specTexID);
    }

    if (!mater.normTexName.isEmpty())
    {
        f->glActiveTexture(GL_TEXTURE0 + M_TEXID + mater.normTexID);
        //        qDebug() << mater.normTexID;
        m_textures[mater.normTexID]->bind();
        m_program->setUniformValue("normalMap", M_TEXID + mater.normTexID);
    }
}

void myGLWidget::setupTextures(Node *node, unsigned int t)
{
    // Texture can only be in the allocated heap!!
    // Setup texture
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load all textures in this node
    for(int imm = 0; imm<node->meshes.size(); ++imm)
    {
        if (node->meshes[imm]->material->Name == QString("DefaultMaterial") ||
                node->meshes[imm]->material->Name == QString("None"))
            continue;
        // Diffuse
        m_texture = new QOpenGLTexture(QImage(modelPath + node->meshes[imm]->material->diffTexName).mirrored());
        node->meshes[imm]->material->diffTexID = texID;
        qDebug() << "Texture loaded: " << node->meshes[imm]->material->diffTexName << "; ID : " << texID;
        texID++;
        m_textures.push_back(m_texture);

        if (!node->meshes[imm]->material->specTexName.isEmpty())
        {
            // Specular
            m_texture = new QOpenGLTexture(QImage(modelPath + node->meshes[imm]->material->specTexName).mirrored());
            node->meshes[imm]->material->specTexID = texID;
            qDebug() << "Texture loaded: " << node->meshes[imm]->material->specTexName << "; ID : " << texID;
            texID++;
            m_textures.push_back(m_texture);
        }
        if (!node->meshes[imm]->material->normTexName.isEmpty())
        {
            // Normal
            m_texture = new QOpenGLTexture(QImage(modelPath + node->meshes[imm]->material->normTexName).mirrored());
            node->meshes[imm]->material->normTexID = texID;
            //            qDebug() << "texID: " << texID;
            qDebug() << "Texture loaded: " << node->meshes[imm]->material->normTexName << "; ID : " << texID;
            texID++;
            m_textures.push_back(m_texture);
        }
    }
    // Recursively load this nodes children nodes
    for(int inn = 0; inn<node->nodes.size(); ++inn)
    {
        setupTextures(&node->nodes[inn], texID);
    }
}

void myGLWidget::setupCubemap()
{
    c_program = new QOpenGLShaderProgram;
    setupShaderProgram(c_program, "src/cubemap.vert", "src/cubemap.frag");

    c_program->bind();

    c_vao.create();
    c_vao.bind();

    c_vbo.create();
    c_vbo.bind();
    c_vbo.allocate(skyboxVertices, 36*3*sizeof(GLfloat));

    // Have to use VAO for layout
    c_program->enableAttributeArray( 0 );      // layout location
    c_program->setAttributeBuffer( 0,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)
    c_vao.release();

    c_cubemap = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    c_cubemap->create();
    c_cubemap->bind();
    //    c_cubemap->setSize(2048, 2048);
    c_cubemap->setSize(512,512);
    c_cubemap->setFormat(QOpenGLTexture::RGB8_UNorm);
    c_cubemap->allocateStorage();

    QString names[] = {
        "right", "left",
        "top" ,"bottom",
        "back", "front"
    };
    QOpenGLTexture::CubeMapFace faces[] = {
        QOpenGLTexture::CubeMapPositiveX,
        QOpenGLTexture::CubeMapNegativeX,
        QOpenGLTexture::CubeMapPositiveY,
        QOpenGLTexture::CubeMapNegativeY,
        QOpenGLTexture::CubeMapPositiveZ,
        QOpenGLTexture::CubeMapNegativeZ
    };
    QString suffix = ".jpg";

    for (int ii = 0; ii < 6; ++ii)
    {
        c_cubemap->setData(0, 0, faces[ii],
                           QOpenGLTexture::RGB,     // Source format
                           QOpenGLTexture::UInt8,   // Source type
                           QImage(cubePath+names[ii]+suffix).
                           convertToFormat(QImage::Format_RGB888).constBits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void myGLWidget::setupStencil()
{
    st_program = new QOpenGLShaderProgram;
    setupShaderProgram(st_program, "src/stencil.vert", "src/stencil.frag");
    m_vao.bind();
    st_program->bind();

    m_vbo.bind();
    st_program->enableAttributeArray( 0 );      // layout location
    st_program->setAttributeBuffer( 0,          // layout location
                                    GL_FLOAT,   // data's type
                                    0,          // Offset to data in buffer
                                    3);         // number of components (3 for x,y,z)

    m_vao.release();
    st_program->release();
}

void myGLWidget::setupCuts()
{
    ct_program = new QOpenGLShaderProgram;
    setupShaderProgram(ct_program, "src/stencil.vert", "src/stencil.frag");

    ct_vao.create();
    ct_vao.bind();

    ct_vbo.create();
    ct_vbo.bind();
    ct_vbo.allocate(quadVertices, 18 * sizeof(float));

    ct_program->bind();

    ct_vbo.bind();
    ct_program->enableAttributeArray( 0 );      // layout location
    ct_program->setAttributeBuffer( 0,          // layout location
                                    GL_FLOAT,   // data's type
                                    0,          // Offset to data in buffer
                                    3);         // number of components (3 for x,y,z)

    ct_program->release();
}

void myGLWidget::setupShadow()
{
    // Configure depth map FBO
    f->glGenFramebuffers(1, &s_depthMapFBO);
    // - Create depth texture
    f->glGenTextures(1, &s_depthMap);
    f->glBindTexture(GL_TEXTURE_2D, s_depthMap);

    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, S_WIDTH, S_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    f->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    f->glBindFramebuffer(GL_FRAMEBUFFER, s_depthMapFBO);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, s_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    s_program = new QOpenGLShaderProgram;
    setupShaderProgram(s_program, "src/shadow.vert", "src/shadow.frag");

    s_program->bind();

    m_vao.bind();
    m_vbo.bind();
    // Have to use VAO for layout
    s_program->enableAttributeArray( 0 );      // layout location
    s_program->setAttributeBuffer( 0,          // layout location
                                   GL_FLOAT,   // data's type
                                   0,          // Offset to data in buffer
                                   3);         // number of components (3 for x,y,z)

    m_vao.release();
    s_program->release();
}

void myGLWidget::renderShadow()
{
    // Render depth of scene to texture (from ligth's perspective)
    // Get light projection/view matrix.
    QMatrix4x4 lightProjection, lightView;
    GLfloat near_plane = 1.0f, far_plane = 7.5f;
    lightProjection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView.lookAt(-ddir, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f));
    s_lightSpaceMatrix = lightProjection * lightView;
    // Now render scene from light's point of view
    s_program->bind();
    s_program->setUniformValue("lightSpaceMatrix", s_lightSpaceMatrix);
    m_model.setToIdentity();

    f->glViewport(0, 0, S_WIDTH, S_HEIGHT);
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    f->glBindFramebuffer(GL_FRAMEBUFFER, s_depthMapFBO);
    s_program->bind();
    f->glClear(GL_DEPTH_BUFFER_BIT);

    m_vao.bind();
    drawShadowNode(s_program, m_rootNode.data(), QMatrix4x4());
    m_vao.release();

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    f->glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    f->glViewport(0, 0, this->width(), this->height());
}

void myGLWidget::initializeGL()
{
    // Initialize debug logger
    //    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    //    Q_ASSERT(ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug")));
    logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &myGLWidget::logMsgHandler);
    logger->startLogging();

    this->initializeOpenGLFunctions();
    // Bind the gl functions to current context
    f = QOpenGLContext::currentContext()->functions();

    m_program = new QOpenGLShaderProgram;

    // Setup default material
    m_materialInfo.diffTexID = 0;
    m_materialInfo.specTexID = 1;
    m_materialInfo.normTexID = 2;
    m_materialInfo.Shininess = 15.0f;
    m_materialInfo.diffTexName = trunkTex;
    m_materialInfo.specTexName = specTex;
    m_materialInfo.normTexName = normTex;

    // 0-2 are reserved for default textures
    texID = 3;

    m_texture = new QOpenGLTexture(QImage(m_materialInfo.diffTexName).mirrored());
    qDebug() << "Texture loaded: " << m_materialInfo.diffTexName << "; ID : " << m_materialInfo.diffTexID;
    m_textures.push_back(m_texture);

    m_texture = new QOpenGLTexture(QImage(m_materialInfo.specTexName).mirrored());
    qDebug() << "Texture loaded: " << m_materialInfo.specTexName << "; ID : " << m_materialInfo.specTexID;
    m_textures.push_back(m_texture);

    m_texture = new QOpenGLTexture(QImage(m_materialInfo.normTexName).mirrored());
    qDebug() << "Texture loaded: " << m_materialInfo.normTexName << "; ID : " << m_materialInfo.normTexID;
    m_textures.push_back(m_texture);

    setupShaderProgram(m_program, "src/objectvert.vsh", "src/objectfrag.fsh");
    setupBuffers();
    setupVertexAttribs();
    setupLighting();
    setupTextures(m_rootNode.data(), 0);
    setupCubemap();
    setupPlane();
    setupStencil();
    setupShadow();
    setupCuts();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_BLEND);
    //    f->glEnable(GL_MULTISAMPLE);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    f->glStencilMask(0x00);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    keys.resize(1024);
}

void myGLWidget::paintGL()
{
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    f->glEnable(GL_DEPTH_TEST);
    f->glDisable(GL_STENCIL_TEST);

    /* Setup all the matrices, etc. */
    paintHelper();
    drawCubemap();
    renderShadow();
    drawPlane();
    //    drawCuts();

    f->glEnable(GL_STENCIL_TEST);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    f->glStencilMask(0xFF);
    //    f->glStencilFunc(GL_ALWAYS, 1, 0xFF);
    f->glClear(GL_STENCIL_BUFFER_BIT);

    drawObject();

    f->glStencilMask(0x00);
    //    f->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //    f->glStencilFunc(GL_GREATER, 1, 0xFF);  // Draw if func(ref&mask) is true
    f->glDisable(GL_DEPTH_TEST);
    f->glEnable(GL_DEPTH_TEST); // Enable depth test to draw current stencil over other branches

    float pointSize = 100.0f/m_camera.Zoom;
    pointSize = pointSize < 1.0f ? 1.0f : pointSize;
    glPointSize(pointSize);  // Adjust point size based on camera fov
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    /* Draw contour here */
    drawScaledObject();

    glPointSize(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    f->glEnable(GL_DEPTH_TEST);
    f->glDisable(GL_STENCIL_TEST);

    if (yRot != lastyRot)
        emit signal_updateAngle();
    lastyRot = yRot;
}

void myGLWidget::paintHelper()
{
    f->glViewport(0, 0, this->width(), this->height());

    m_view.setToIdentity();

    /* LOOKAT */
    keyMovement();
    m_view = m_camera.GetViewMatrix(m_view);

    // m_projection and m_view are universal
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.Zoom, (float)this->width()/(float)this->height(), 0.1f, 100.0f);
}

void myGLWidget::drawPlane()
{
    p_program->bind();

    /* Draw plane here */
    p_program->setUniformValue("view", m_view);
    p_program->setUniformValue("projection", m_projection);
    p_program->setUniformValue("planeTex", S_TEXID);
    p_program->setUniformValue("lightSpaceMatrix", s_lightSpaceMatrix);
    p_program->setUniformValue("shadowMap", s_depthMap);

    f->glActiveTexture(GL_TEXTURE0 + S_TEXID);
    f->glEnable(GL_TEXTURE_2D);
    p_texture->bind();

    // Draw shadow here
    f->glActiveTexture(GL_TEXTURE0 + s_depthMap);
    f->glBindTexture(GL_TEXTURE_2D, s_depthMap);

    p_vao.bind();
    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    p_vao.release();

    p_program->release();
}

void myGLWidget::drawShadowNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix)
{
    // Layers of transformation: branch to trunk, sub branch to branch, etc...
    objectMatrix *= node->transformation;
    QMatrix4x4 modelMatrix = m_model * objectMatrix;

    program->setUniformValue("model", modelMatrix);

    // Draw each mesh in this node
    if (node->show)
    {
        for(int imm = 0; imm<node->meshes.size(); ++imm)
        {
            if (isbranch(node->nodeID) && (*m_deleted)[ST2IDX(node->nodeID)])
                continue;
            if (isbranch(node->nodeID) && *m_analyze &&
                    !(*m_selected)[ST2IDX(node->nodeID)])
                continue;
            glDrawElements( GL_TRIANGLES,
                            node->meshes[imm]->indexCount,
                            GL_UNSIGNED_INT,
                            (const void*)(node->meshes[imm]->indexOffset * sizeof(unsigned int)));
        }
    }

    // Recursively draw this nodes children nodes
    for(int inn = 0; inn<node->nodes.size(); ++inn)
        drawShadowNode(program, &node->nodes[inn], objectMatrix);
}

void myGLWidget::drawCuts()
{
    ct_program->bind();

    ct_program->setUniformValue("view", m_view);
    ct_program->setUniformValue("projection", m_projection);

    QMatrix4x4 trans = m_rootNode.data()->transformation;

    QMatrix4x4 ct_model;
    int focus = 0;
    // Cant do it right now since the only dataset is offsetted by something weird distance
    for (int i = 0; i < thisManyBranches; ++i)
    {
        //        QVector3D cutPos(branches[i].cut.x,branches[i].cut.y,branches[i].cut.z);
        for (int j = 0; j < branches[i].circles.x.size(); ++j) {
            QVector3D cutPos(branches[i].circles.x[j],branches[i].circles.z[j],branches[i].circles.y[j]);

            float sigma = qAtan(branches[i].cutNorm.x()/branches[i].cutNorm.z());
            float phi = qAtan(branches[i].cutNorm.z()/branches[i].cutNorm.y());
            QMatrix4x4 trans1, trans2; // identity matrix

            // Have problem when normal has 0s
            trans1.rotate(-phi, QVector3D(1.0f, 0.0f, 0.0f));
            trans2.rotate(-sigma, QVector3D(0.0f, 1.0f, 0.0f));


            // Do scaling first then translation
            ct_model.setToIdentity();

            //            ct_model = trans * ct_model;

            ct_model.translate(cutPos);
            ct_model = trans2 * trans1 * ct_model;

            ct_model.scale(0.5f);

            //        ct_model.translate(branches[4].cut.x - 0.25,
            //                           branches[4].cut.y + 1.75,
            //                           branches[4].cut.z - 1.0);

            ct_model.setToIdentity();
            ct_model.translate(QVector3D(-0.25f*0.5, 0.5, 2.25f));

            ct_program->setUniformValue("model", ct_model);
            if (*m_current == i) focus = 1;
            else focus = 0;
            ct_program->setUniformValue("focus", focus);

            ct_vao.bind();
            f->glDrawArrays(GL_TRIANGLES, 0, 18);
            ct_vao.release();
        }


    }

    ct_program->release();
}

void myGLWidget::drawScaledObject()
{
    st_program->bind();

    st_program->setUniformValue("view", m_view);
    st_program->setUniformValue("projection", m_projection);

    m_vao.bind();
    drawScaledNode(st_program, m_rootNode.data(), QMatrix4x4());
    m_vao.release();

    st_program->release();
}

void myGLWidget::drawScaledNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix)
{
    // Layers of transformation: branch to trunk, sub branch to branch, etc...
    objectMatrix *= node->transformation;
    QMatrix4x4 modelMatrix = m_model * objectMatrix;

    program->setUniformValue(program->uniformLocation("model"), modelMatrix);

    // Draw each mesh in this node
    // Node must be a branch, selected and not deleted
    // If highlight, draw regardless
    if (node->show && isbranch(node->nodeID) &&
            (
                ((*m_selected)[ST2IDX(node->nodeID)]
                 && !(*m_deleted)[ST2IDX(node->nodeID)])
                || *m_highlight
                )
            )
    {
        for(int imm = 0; imm<node->meshes.size(); ++imm)
        {
            int focus = 0;
            if (ST2IDX(node->nodeID) == *m_current) focus = 1;
            if (*m_highlight)
            {
                focus = 2;
                int rank = getBranchRadiusRank(sev_rank, ST2IDX(node->nodeID));
                st_program->setUniformValue("color_in",
                                            colorGradient[rank]);
            }
            st_program->setUniformValue("focus", focus);
            // f->glStencilFunc(GL_EQUAL, node->nodeID, 0xFF);
            f->glStencilFunc(GL_NOTEQUAL, node->nodeID, 0xFF);  // Draw if not overlapping with this branch

            glDrawElements( GL_TRIANGLES,
                            node->meshes[imm]->indexCount,
                            GL_UNSIGNED_INT,
                            (const void*)(node->meshes[imm]->indexOffset * sizeof(unsigned int)));
        }
    }

    // Recursively draw this nodes children nodes
    for(int inn = 0; inn<node->nodes.size(); ++inn)
        drawScaledNode(program, &node->nodes[inn], objectMatrix);
}

void myGLWidget::drawObject()
{
    m_program->bind();

    m_program->setUniformValue("viewPosition", m_camera.Position);
    m_program->setUniformValue("view", m_view);
    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("lightSpaceMatrix", s_lightSpaceMatrix);
    m_program->setUniformValue("shadowMap", s_depthMap);

    m_model.setToIdentity();
    //    m_model.rotate(myTime.elapsed()/25.0f , QVector3D(0.0f, 1.0f, 0.0f));

    // Draw shadow here
    f->glActiveTexture(GL_TEXTURE0 + s_depthMap);
    f->glBindTexture(GL_TEXTURE_2D, s_depthMap);

    m_vao.bind();
    drawNode(m_program, m_rootNode.data(), QMatrix4x4());
    m_vao.release();

    m_program->release();
}

void myGLWidget::drawNode(QOpenGLShaderProgram *program, const Node *node, QMatrix4x4 objectMatrix)
{
    // Layers of transformation: branch to trunk, sub branch to branch, etc...
    objectMatrix *= node->transformation;
    QMatrix4x4 modelMatrix = m_model * objectMatrix;

    program->setUniformValue("model", modelMatrix);

    // Draw each mesh in this node
    if (node->show)
    {
        for(int imm = 0; imm<node->meshes.size(); ++imm)
        {
            if (node->meshes[imm]->material->Name == QString("DefaultMaterial") ||
                    node->meshes[imm]->material->Name == QString("None"))
                setMaterialUniforms(m_materialInfo);
            else
                setMaterialUniforms(*node->meshes[imm]->material);

            // Assign a unique stencil value for each object
            f->glStencilFunc(GL_ALWAYS, node->nodeID, 0xFF);

            if (isbranch(node->nodeID) && (*m_deleted)[ST2IDX(node->nodeID)])
                continue;
            if (isbranch(node->nodeID) && *m_analyze &&
                    !(*m_selected)[ST2IDX(node->nodeID)])
                continue;
            glDrawElements( GL_TRIANGLES,
                            node->meshes[imm]->indexCount,
                            GL_UNSIGNED_INT,
                            (const void*)(node->meshes[imm]->indexOffset * sizeof(unsigned int)));
        }
    }

    // Recursively draw this nodes children nodes
    for(int inn = 0; inn<node->nodes.size(); ++inn)
        drawNode(program, &node->nodes[inn], objectMatrix);
}

void myGLWidget::drawCubemap()
{
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL);

    c_program->bind();
    f->glDepthMask(GL_FALSE); // Disable writing to depth buffer so other objects can be shown

    c_program->setUniformValue(c_program->uniformLocation("view"), m_view);
    c_program->setUniformValue(c_program->uniformLocation("projection"), m_projection);
    f->glActiveTexture(GL_TEXTURE0 + C_TEXID);
    c_cubemap->bind();
    c_program->setUniformValue("skybox", C_TEXID);

    c_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    c_vao.release();

    f->glDepthMask(GL_TRUE);
    f->glDepthFunc(GL_LESS);
    c_program->release();
}

QVector<QVector<float> > myGLWidget::XYZtoUV(QVector<float> vertices)
{
    QVector<QVector<float> > textureUV;
    QVector<float> UV;
    float scale = 3.0f;
    // Size must be multiples of 3
    if (vertices.size() % 3 != 0)
    {
        return textureUV;
    }

    for(int i=0; i<vertices.size(); i+=3)
    {
        // U = phi, V = y
        float phi = qAtan(vertices[i+2]/vertices[i]);
        UV.push_back(scale*phi);
        UV.push_back(scale*vertices[i+1]);
    }
    textureUV.push_back(UV);
    return textureUV;
}

void myGLWidget::logMsgHandler(const QOpenGLDebugMessage &msg)
{
    //            qDebug() << msg;
}

void myGLWidget::mousePressEvent(QMouseEvent *event)
{
    firstMouse = true;
    clickX = event->x();
    clickY = this->height() - event->y() - 1;
    makeCurrent();
    f->glReadPixels(clickX, clickY, 1, 1,
                    GL_STENCIL_INDEX, GL_UNSIGNED_INT, &clickIndex);
    qDebug() << "Picked: " << clickIndex;
    doneCurrent();

    if (isbranch(clickIndex))
    {
        // In case of already selected, make current null
        // If not selected, simply make it current
        int alreadySelected = (*m_selected)[ST2IDX(clickIndex)];
        if (alreadySelected)
        {
            if (*m_current == ST2IDX(clickIndex))
            {
                *m_current = -1;
                (*m_selected)[ST2IDX(clickIndex)] ^= 1;

            }
            else
            {
                *m_current = ST2IDX(clickIndex);
            }
        }
        else
        {
            *m_current = ST2IDX(clickIndex);
            (*m_selected)[ST2IDX(clickIndex)] ^= 1;
        }
        emit signal_updateInfo();
    }
}

void myGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons())
        return;
    if (firstMouse)
    {
        lastX = event->pos().x();
        lastY = event->pos().y();
        firstMouse = false;
    }
    GLfloat xOffset = event->pos().x() - lastX;
    GLfloat yOffset = event->pos().y() - lastY;
    lastX = event->pos().x();
    lastY = event->pos().y();
    m_camera.ProcessMouseMovement(xOffset, yOffset);
    yRot = m_camera.yRot;
}

void myGLWidget::wheelEvent(QWheelEvent *event)
{
    // One delta is a multiple of 120
    m_camera.ProcessMouseScroll(event->delta()/60.0f);
}

void myGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() < keys.size())
        keys[event->key()] = true;
}

void myGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() < keys.size())
        keys[event->key()] = false;
}

void myGLWidget::keyMovement()
{
    if (keys[Qt::Key_W])
        m_camera.ProcessKeyboard(FORWARD);
    if (keys[Qt::Key_S])
        m_camera.ProcessKeyboard(BACKWARD);
    if (keys[Qt::Key_A])
        m_camera.ProcessKeyboard(LEFT);
    if (keys[Qt::Key_D])
        m_camera.ProcessKeyboard(RIGHT);
}

bool myGLWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        GLint hoverX, hoverY;
        hoverX = helpEvent->x();
        hoverY = this->height() - helpEvent->y() - 1;
        makeCurrent();
        int hoverIndex;
        f->glReadPixels(hoverX, hoverY,
                        1, 1,
                        GL_STENCIL_INDEX, GL_UNSIGNED_INT, &hoverIndex);
        qDebug() << "Hovering on: " << hoverIndex;
        doneCurrent();

        // Show tooltip if branch is not deleted
        if (isbranch(hoverIndex) && !(*m_deleted)[ST2IDX(hoverIndex)])
        {
            QString info = QString("Branch Info:      \n");
            info += QString("diameter(cm): ") +
                    QString::number(2*m2cm(branches[ST2IDX(hoverIndex)].circles.r[0]), 'g', 3) +
                    QString("\n");
            info += QString("angle(deg): ") +
                    QString::number(branches[ST2IDX(hoverIndex)].angle, 'g', 3) +
                    QString("\n");
            info += QString("ratio: ") +
                    QString::number(branches[ST2IDX(hoverIndex)].BTratio, 'g', 3);
            QToolTip::showText(helpEvent->globalPos(), info);
        }
        else
        {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    }
    return QWidget::event(event);
}

void myGLWidget::resizeGL(int width, int height)
{
    f->glViewport(0, 0, width, height);
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.Zoom, (float)width/height, 0.1f, 100.0f);
}

void myGLWidget::setupSelection(QVector<int> *sel, QVector<int> *del, int *curr,
                                int *anal, int *hlgt)
{
    m_selected = sel;
    m_deleted = del;
    m_current = curr;
    m_analyze = anal;
    m_highlight = hlgt;
}
