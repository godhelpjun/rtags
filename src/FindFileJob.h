#ifndef FindFileJob_h
#define FindFileJob_h

#include "String.h"
#include "List.h"
#include "RTagsClang.h"
#include "Job.h"
#include "Location.h"
#include "RegExp.h"

class FindFileJob : public Job
{
public:
    FindFileJob(const QueryMessage &query, const shared_ptr<Project> &project);
protected:
    virtual void execute();
private:
    String mPattern;
    RegExp mRegExp;
};

#endif
