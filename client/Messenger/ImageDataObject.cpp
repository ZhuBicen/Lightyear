// ImageDataObject.h: Impementation for IDataObject Interface to be used 
//					     in inserting bitmap to the RichEdit Control.
//
// Author : Hani Atassi  (atassi@arabteam2000.com)
//
// How to use : Just call the static member InsertBitmap with 
//				the appropriate parrameters. 
//
// Known bugs :
//
//
//////////////////////////////////////////////////////////////////////
#include <RichEdit.H>
#include <Ole2.h>
#include <richole.h>
#include <assert.h>
#include "ImageDataObject.hpp"

#include "Log.hpp"
#include "../DuiLib/UIlib.h"
using namespace DuiLib;

void CImageDataObject::InsertImage(IRichEditOle* pRichEditOle, HBITMAP p_hbitmap){
    SCODE sc;

    // Get the image data object
    //
    CImageDataObject *pods = new CImageDataObject;

    // LPDATAOBJECT lpDataObject;
    // pods->QueryInterface(IID_IDataObject, (void **)&lpDataObject);

    pods->SetBitmap(p_hbitmap);

    // Get the RichEdit container site
    //
    IOleClientSite *pOleClientSite;	
    pRichEditOle->GetClientSite(&pOleClientSite);

    // Initialize a Storage Object
    //
    IStorage *pStorage;	

    LPLOCKBYTES lpLockBytes = NULL;
    sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
    if (sc != S_OK)
    {

        MLOG(logERROR) << __FUNCTION__ << "CreateILockBytesOnHGlobal error.";
        return;
    }
    assert(lpLockBytes != NULL);

    sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
        STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
    if (sc != S_OK)
    {
        assert(lpLockBytes->Release() == 0);
        lpLockBytes = NULL;
        MLOG(logERROR) << __FUNCTION__ << "StgCreateDocfileOnILockBytes";
        return;
    }
    assert(pStorage != NULL);

    // The final ole object which will be inserted in the richedit control
    //
    IOleObject *pOleObject; 
    pOleObject = pods->GetOleObject(pOleClientSite, pStorage);

    // all items are "contained" -- this makes our reference to this object
    //  weak -- which is needed for links to embedding silent update.
    OleSetContainedObject(pOleObject, TRUE);

    // Now Add the object to the RichEdit 
    //
    REOBJECT reobject;
    ZeroMemory(&reobject, sizeof(REOBJECT));
    reobject.cbStruct = sizeof(REOBJECT);

    CLSID clsid;
    sc = pOleObject->GetUserClassID(&clsid);
    if (sc != S_OK){

        MLOG(logERROR) << __FUNCTION__ << "GetUserClassID.";
        return;
    }

    reobject.clsid = clsid;
    reobject.cp = REO_CP_SELECTION;
    reobject.dvaspect = DVASPECT_CONTENT;
    reobject.poleobj = pOleObject;
    reobject.polesite = pOleClientSite;
    reobject.pstg = pStorage;

    // Insert the bitmap at the current location in the richedit control
    //
    pRichEditOle->InsertObject(&reobject);

    // Release all unnecessary interfaces
    //
    pOleObject->Release();
    pOleClientSite->Release();
    pStorage->Release();
    // lpDataObject->Release();
}


//////////////////////////////////////////////////////////////////////
// Static member functions
//////////////////////////////////////////////////////////////////////
void CImageDataObject::InsertImage(IRichEditOle* pRichEditOle, const std::wstring& imagePath)
{
    auto hBitmap = CRenderEngine::LoadImage(imagePath.c_str())->hBitmap;
    InsertImage(pRichEditOle, hBitmap);

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	assert(hBitmap);

	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle		
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL;				// Use ReleaseStgMedium

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
	fm.ptd = NULL;							// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
	fm.lindex = -1;							// Index = Not applicaple
	fm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle

	this->SetData(&fm, &stgm, TRUE);		
}

IOleObject *CImageDataObject::GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage)
{
	assert(m_stgmed.hBitmap);

	SCODE sc;
	IOleObject *pOleObject;
	sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, 
			&m_fromat, pOleClientSite, pStorage, (void **)&pOleObject);
	if (sc != S_OK) {
        return NULL;
    }
    return pOleObject;
}
