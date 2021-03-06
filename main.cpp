/*
 * Author:Gxin
 * main
 */
#include "dcview.h"
#include "glkernel/glextensions.h"
#include <QApplication>


inline bool matchString(const char *extensionString, const char *subString)
{
    int subStringLength = strlen(subString);
    return (strncmp(extensionString, subString, subStringLength) == 0)
            && ((extensionString[subStringLength] == ' ') || (extensionString[subStringLength] == '\0'));
}

bool necessaryExtensionsSupported()
{
    const char *extensionString = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    const char *p = extensionString;

    const int GL_EXT_FBO = 1;
    const int GL_ARB_VS = 2;
    const int GL_ARB_FS = 4;
    const int GL_ARB_SO = 8;
    int extensions = 0;

    while (*p) {
        if (matchString(p, "GL_EXT_framebuffer_object"))
            extensions |= GL_EXT_FBO;
        else if (matchString(p, "GL_ARB_vertex_shader"))
            extensions |= GL_ARB_VS;
        else if (matchString(p, "GL_ARB_fragment_shader"))
            extensions |= GL_ARB_FS;
        else if (matchString(p, "GL_ARB_shader_objects"))
            extensions |= GL_ARB_SO;
        while ((*p != ' ') && (*p != '\0'))
            ++p;
        if (*p == ' ')
            ++p;
    }
    return (extensions == 15);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //OpenGL支持检测
    if((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)==0){
        QMessageBox::critical(0, "OpenGL features missing",
                              "OpenGL version 1.5 or higher is required to run this game.\n"
                              "The program will now exit.");
        exit(-1);
    }

    QGLWidget *widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    widget->makeCurrent();

    if (!necessaryExtensionsSupported()) {
        QMessageBox::critical(0, "OpenGL features missing",
                              "The OpenGL extensions required to run this game are missing.\n"
                              "The program will now exit.");
        delete widget;
        exit(-2);
    }

    // 检查所有需要的功能
    if (!getGLExtensionFunctions().resolve(widget->context())) {
        QMessageBox::critical(0, "OpenGL features missing",
                              "Failed to resolve OpenGL functions required to run this game.\n"
                              "The program will now exit.");
        delete widget;
        exit(-3);
    }

    widget->makeCurrent();

    DCView view;
    view.setViewport(widget);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.show();

    return app.exec();
}
