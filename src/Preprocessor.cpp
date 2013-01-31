#include "Preprocessor.h"
#include "Connection.h"
#include "Process.h"
#include "Log.h"
#include "RTags.h"

Preprocessor::Preprocessor(const SourceInformation &args, uint8_t buildIndex, Connection *connection)
    : mArgs(args), mBuildIndex(buildIndex), mConnection(connection), mProc(0)
{
    mProc = new Process;
    mProc->finished().connect(this, &Preprocessor::onProcessFinished);
}

Preprocessor::~Preprocessor()
{
    delete mProc;
}

void Preprocessor::preprocess()
{
    List<String> args = mArgs.builds.at(mBuildIndex).args;
    args.append("-E");
    args.append(mArgs.sourceFile);
    mProc->start(mArgs.builds.at(mBuildIndex).compiler, args);
}

void Preprocessor::onProcessFinished()
{
    mConnection->write<256>("// %s %s", mArgs.builds.at(mBuildIndex).compiler.constData(),
                            String::join(mArgs.builds.at(mBuildIndex).args, ' ').constData());
    mConnection->write(mProc->readAllStdOut());
    const String err = mProc->readAllStdErr();
    if (!err.isEmpty()) {
        mConnection->write<1024>("/* %s */", err.constData());
    }
    mConnection->finish();
    deleteLater();
}
