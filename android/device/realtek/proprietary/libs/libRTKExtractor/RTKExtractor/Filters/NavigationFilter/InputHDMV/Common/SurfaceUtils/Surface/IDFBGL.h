#ifndef IDFBGL_H
#define IDFBGL_H

#include "IDFBBase.h"
#include "directfbgl.h"
	
class IDFBGL : public IDFBBase<IDirectFBGL>
{
public:
	IDFBGL(IDirectFBGL* dfbif = 0) : IDFBBase <IDirectFBGL> (dfbif) {}

	void	Lock() const;
	void	Unlock() const;
	void	GetAttributes(DFBGLAttributes &attributes) const;
};

#endif	//IDFBGL_H
