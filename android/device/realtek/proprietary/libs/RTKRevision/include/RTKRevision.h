#ifndef RTKREVISION_H
#define RTKREVISION_H

char * GetGitSha() {
#ifdef GIT_VERSION
    return (char *) GIT_VERSION;
#else
    return NULL;
#endif
}

#endif /* End of RTKREVISION_H */
