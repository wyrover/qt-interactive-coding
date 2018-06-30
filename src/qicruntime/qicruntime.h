#ifndef QICRUNTIME_H
#define QICRUNTIME_H

#include <QString>

#if defined QIC_DLL
#define QIC_EXPORT Q_DECL_EXPORT
#else
#define QIC_EXPORT Q_DECL_IMPORT
#endif

class qicRuntimePrivate;

/**
    The qicRuntime class provides the runtime build and execution environment.
    It utilizes the locally installed Qt build system `qmake` and native build
    toolchain.

    The exec() method takes a piece of C++ source code, wraps it in a shared
    library `qmake` project and builds using the installed C++ toolchain and Qt
    SDK. The code passed to this function must define and export the qic_entry()
    function. Upon successful compilation of this code into a shared library,
    this library is loaded, and the qic_entry() function is called.

    Use the setCtxVar() method to pass data to the runtime-compiled code and
    getCtxVar() to retrieve data created by the runtime code.

    Use the various setters to control the build environment. You can override
    environment variables, path to the `qmake` and `make` programs, add defines,
    include paths and linked libraries. By default, the compiled library does
    not link with Qt. You can override and link with Qt libraries using
    setQtLibs(). Use setQtConfig() to configure additional Qt features (debug
    build, exceptions, rtti).

    The loadEnv() method comes in handy, if you need to replicate a build
    environment that is not your default command line environment. It loads a
    set of environment variables from a file. You can easily take a snapshot of
    your build environment using the `env` command on Linux and `set` command
    on Windows and then replicate this build environment in qicRuntime. To do
    this, open a terminal, configure your build environment, then grab the
    output of `env` or `set` into a file and load this file using loadEnv().
 */
class QIC_EXPORT qicRuntime
{
public:
    qicRuntime();
    ~qicRuntime();

    //TODO: make exec() async, return a quasi future object qicExecutable

    bool exec(QString source);
    bool execFile(QString filename);

    // build env

    void setEnv(QString name, QString value);
    void addEnv(QString name, QString value);
    void loadEnv(QString path);
    void setQmake(QString path);
    void setMake(QString path);
    void setDefines(QStringList defines);
    void setIncludePath(QStringList dirs);
    void setLibs(QStringList libs);
    void setQtLibs(QStringList qtlibs);
    void setQtConfig(QStringList qtconf);

    // ctx

    void *getCtxVar(const char *name);
    void *setCtxVar(void *ptr, const char *name, void(*deleter)(void*));

private:
    bool compile(QString src);

private:
    qicRuntimePrivate *p;
};

#endif // QICRUNTIME_H
