//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "xtechexp.h"
#include "i2dAtlas.h"


#include "triobj.h"
#include "impexp.h"

#include "Max.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "helpsys.h"
#include "buildver.h"
#include "strbasic.h"

#include "IGame.h"
#include "IGameObject.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "IGameError.h"
#include "IGameFX.h"
#include "Skeleton.h"
#include <map>
#include "wm3.h"



#define xtechexp_CLASS_ID	Class_ID(0x6c746774, 0x4e49d519)

class xtechexp : public SceneExport {
public:
    //Constructor/Destructor
    xtechexp();
    ~xtechexp();
    
    
    Bitmap *atlasBM;
    BitmapInfo atlasInfo;
    I2dAtlas* atlas;
    std::map<std::string,I2dAtlas::I2dNode*> atlasInfoMap;
    
    
    int extensionID;
    
    
    FILE        *   stream;
    IGameScene  *   pIgame;
    int				ExtCount();					// Number of extensions supported
    const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
    const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
    const TCHAR *	AuthorName();				// ASCII Author name
    const TCHAR *	CopyrightMessage();			// ASCII Copyright message
    const TCHAR *	OtherMessage1();			// Other message #1
    const TCHAR *	OtherMessage2();			// Other message #2
    unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
    void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
    XMESHFlags xmeshFlags;
    BOOL SupportsOptions(int ext, DWORD options);
    int  DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
    int ExportChildNodeInfo(IGameNode*child, Interface* ip,std::vector<long>*offsets,std::vector<std::string>*names,std::string*paletText, std::vector<long>*sizeMeshs);
    
    int DumpMesh(IGameObject*obj,IGameNode*node, Interface* ip,std::vector<std::string>*names,std::string*paletText);
    int DumpAnim(IGameObject*obj,IGameNode*node, Interface* ip,std::vector<std::string>*names,std::string*paletText);
    void enumMeshs(IGameNode*child,int*numMeshs);

    void writeVertexMorphInfo(MeshVertex& vertx,int channelID);
    int writeVert(MeshVertex& vertx,std::string*paletText);
    void DumpProperty(TCHAR* propName, IGameProperty * prop);
    void makeValidURIFilename(TSTR& fn, bool stripMapPaths=false);
    I2dAtlas::I2dNode * addBitmapToAtlas(TSTR pathImage, Interface* ip);
};



class xtechexpClassDesc : public ClassDesc2
{
public:
    virtual int IsPublic() 							{ return TRUE; }
    virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new xtechexp(); }
    virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
    virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
    virtual Class_ID ClassID() 						{ return xtechexp_CLASS_ID; }
    virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }
    
    virtual const TCHAR* InternalName() 			{ return _T("xtechexp"); }	// returns fixed parsable name (scripter-visible name)
    virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
    
    
};


ClassDesc2* GetxtechexpDesc() {
    static xtechexpClassDesc xtechexpDesc;
    return &xtechexpDesc;
}





INT_PTR CALLBACK xtechexpOptionsDlgProc(HWND hWnd,UINT message,WPARAM,LPARAM lParam) {
    static xtechexp* imp = nullptr;
    
    switch(message) {
        case WM_INITDIALOG:
            imp = (xtechexp *)lParam;
            CenterWindow(hWnd,GetParent(hWnd));
            return TRUE;
            
        case WM_CLOSE:
            EndDialog(hWnd, 0);
            return 1;
    }
    return 0;
}


//--- xtechexp -------------------------------------------------------
xtechexp::xtechexp()
{
    
}

xtechexp::~xtechexp()
{
    
}

int xtechexp::ExtCount()
{
    //#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
    return 2;
}

const TCHAR *xtechexp::Ext(int i)
{
    if (i == 0)
        return _T("xmesh");
    else
        return _T("XMeshAni");
}

const TCHAR *xtechexp::LongDesc()
{
    //#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
    return _T("Mesh for XTech engine");
}

const TCHAR *xtechexp::ShortDesc()
{
    //#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
    return _T("XMESH");
}

const TCHAR *xtechexp::AuthorName()
{
    //#pragma message(TODO("Return ASCII Author name"))
    return _T("Easy proger");
}

const TCHAR *xtechexp::CopyrightMessage()
{
    //#pragma message(TODO("Return ASCII Copyright message"))
    return _T("XTech engine easy proger Copyright");
}

const TCHAR *xtechexp::OtherMessage1()
{
    //TODO: Return Other message #1 if any
    return _T("");
}

const TCHAR *xtechexp::OtherMessage2()
{
    //TODO: Return other message #2 in any
    return _T("");
}

unsigned int xtechexp::Version()
{
    //#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
    return 101;
}

void xtechexp::ShowAbout(HWND /*hWnd*/)
{
    // Optional
}



class WorkFile {
private:
    FILE *stream;
    
public:
    WorkFile(const TCHAR *filename,const TCHAR *mode) { stream = NULL; Open(filename, mode); };
    ~WorkFile() { Close(); };
    FILE *			Stream() { return stream; };
    int				Close() { int result=0; if(stream) result=fclose(stream); stream = NULL; return result; }
    void			Open(const TCHAR *filename,const TCHAR *mode)
    {
        Close();
        // for a360 support - allows binary diff syncing
        if (mode != nullptr && (mode[0] == _T('w') || mode[0] == _T('a') || (mode[0] == _T('r') && mode[1] == _T('+') )))
        {
            MaxSDK::Util::Path storageNamePath(filename);
            storageNamePath.SaveBaseFile();
        }
        stream = _tfopen(filename,mode);
    }
};

// Handy memory worker

class Memory {
    void *ptr;
public:
    Memory() { ptr = NULL; }
    Memory(int amount, BOOL zero = FALSE) { ptr = NULL; Alloc(amount, zero); }
    ~Memory() { Free(); }
    void *			Ptr() { return ptr; }
    void *			Realloc(int amount);
    void *			Alloc(int amount, BOOL zero = FALSE);
    void			Free() { if(ptr) free(ptr); ptr = NULL; }
};

void *Memory::Realloc(int amount) {
    if(ptr)
        ptr = realloc(ptr, amount);
    else
        ptr = malloc(amount);
    return ptr;
}

void *Memory::Alloc(int amount, BOOL zero) {
    Free();
    ptr = malloc(amount);
    if(ptr && zero) {
        char *p = (char *)ptr;
        for(int i = 0; i < amount; ++i)
            *p++ = 0;
    }
    return ptr;
}


BOOL xtechexp::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
    //#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
    return TRUE;
}
static void MessageBox(int s1, int s2) {
    TSTR str1(GetString(s1));
    TSTR str2(GetString(s2));
    MessageBox(GetActiveWindow(), str1.data(), str2.data(), MB_OK);
}

static int MessageBox(int s1, int s2, int option = MB_OK) {
    TSTR str1(GetString(s1));
    TSTR str2(GetString(s2));
    return MessageBox(GetActiveWindow(), str1, str2, option);
}

static int Alert(int s1, int s2 = 0, int option = MB_OK) {
    return MessageBox(s1, s2, option);
}


SceneEntry::SceneEntry(INode *n, Object *o, int t) {
    node = n; obj = o; type = t; next = NULL;
    tnode = n->GetTarget();
}
class SceneEnumProc : public ITreeEnumProc {
public:
    Interface	*i;
    SceneEntry *head;
    SceneEntry *tail;
    IScene		*theScene;
    int			count;
    TimeValue	time;
    SceneEnumProc(IScene *scene, TimeValue t, Interface *i);
    ~SceneEnumProc();
    int			Count() { return count; }
    void		Append(INode *node, Object *obj, int type);
    int			callback( INode *node );
    Box3		Bound();
    SceneEntry *Find(INode *node);
    //		SceneEntry *operator[](int index);
    void BuildNames();
};

SceneEnumProc::SceneEnumProc(IScene *scene, TimeValue t, Interface *i) {
    time = t;
    theScene = scene;
    count = 0;
    head = tail = NULL;
    this->i = i;
    theScene->EnumTree(this);
}

SceneEnumProc::~SceneEnumProc() {
    while(head) {
        SceneEntry *next = head->next;
        delete head;
        head = next;
    }
    head = tail = NULL;
    count = 0;
}

int SceneEnumProc::callback(INode *node) {
    if(node->Selected() == FALSE)
        return TREE_CONTINUE;
    Object *obj = node->EvalWorldState(time).obj;
    if (obj->CanConvertToType(triObjectClassID)) {
        Append(node, obj, OBTYPE_MESH);
        return TREE_CONTINUE;
    }
    
    
    
    return TREE_CONTINUE;	// Keep on enumeratin'!
}


void SceneEnumProc::Append(INode *node, Object *obj, int type) {
    SceneEntry *entry = new SceneEntry(node, obj, type);
    
    if(tail)
        tail->next = entry;
    tail = entry;
    if(!head)
        head = entry;
    count++;
}

Box3 SceneEnumProc::Bound() {
    Box3 bound;
    bound.Init();
    SceneEntry *e = head;
    ViewExp& vpt = i->GetViewExp(NULL);
    if ( ! vpt.IsAlive() )
    {
        // why are we here
        DbgAssert(!_T("Invalid viewport!"));
        return Box3();
    }
    
    while(e) {
        Box3 bb;
        e->obj->GetWorldBoundBox(time, e->node, vpt.ToPointer(), bb);
        bound += bb;
        e = e->next;
    }
    return bound;
}

SceneEntry *SceneEnumProc::Find(INode *node) {
    SceneEntry *e = head;
    while(e) {
        if(e->node == node)
            return e;
        e = e->next;
    }
    return NULL;
}

class ObjName {
public:
    TSTR name;
    ObjName *next;
    ObjName(TSTR n) { name = n; next = NULL; }
};

class ObjNameList {
public:
    ObjName *head;
    ObjName *tail;
    int			count;
    ObjNameList() { head = tail = NULL; count = 0; }
    ~ObjNameList();
    int			Count() { return count; }
    int			Contains(TSTR &n);
    void		Append(TSTR &n);
    void		MakeUnique(TSTR &n);
};

ObjNameList::~ObjNameList() {
    while(head) {
        ObjName *next = head->next;
        delete head;
        head = next;
    }
    head = tail = NULL;
    count = 0;
}

int ObjNameList::Contains(TSTR &n) {
    ObjName *e = head;
    int index = 0;
    while(e) {
        if(e->name == n)
            return index;
        e = e->next;
        index++;
    }
    return -1;
}

void ObjNameList::Append(TSTR &n) {
    ObjName *entry = new ObjName(n);
    if(tail)
        tail->next = entry;
    tail = entry;
    if(!head)
        head = entry;
    count++;
}

void ObjNameList::MakeUnique(TSTR &n) {
    // First make it less than 10 chars.
    if (n.Length()>10) n.Resize(10);
    
    if(Contains(n) < 0) {
        Append(n);
        return;
    }
    // Make it unique and keep it 10 chars or less
    int workLen = n.Length();
    for(int i = 0; i < 100000; ++i) {
        TCHAR num[12];
        _sntprintf(num,12,_T("%d"),i);
        TSTR work = n;
        int totlen = (int)_tcslen(num) + workLen;
        if(totlen > 10)
            work.Resize(10 - (totlen - 10));
        work = work + num;
        if(Contains(work) < 0) {
            Append(work);
            n = work;
            return;
        }
    }
    // Forget it!
}

ObjNameList theObjNames;
void SceneEnumProc::BuildNames()
{
    ObjNameList nameList;
    SceneEntry *ptr = head;
    
    while (ptr) {
        ptr->name = ptr->node->GetName();
        nameList.MakeUnique(ptr->name);
        ptr = ptr->next;
    }
}


class ObjectList {
public:
    ObjectEntry *head;
    ObjectEntry *tail;
    int			count;
    ObjectList(SceneEnumProc &scene);
    ~ObjectList();
    int			Count() { return count; }
    void		Append(SceneEntry *e);
    ObjectEntry *Contains(Object *obj);
    ObjectEntry *Contains(INode *node);
    INode		*FindLookatNode(INode *node);
};

ObjectList::ObjectList(SceneEnumProc &scene) {
    head = tail = NULL;
    count = 0;
    // Zip thru the object list and record all unique objects (Some may be used by more than one node)
    int scount = scene.Count();
    for(SceneEntry *se = scene.head; se!=NULL; se = se->next) {
        // can't multiple instance lights and cameras in 3DS
        // so make them all unique--DS 4/6/96
        if ( (se->type!=OBTYPE_MESH)|| !Contains(se->obj))
            Append(se);
    }
}

ObjectList::~ObjectList() {
    while(head) {
        ObjectEntry *next = head->next;
        delete head;
        head = next;
    }
    head = tail = NULL;
    count = 0;
}

ObjectEntry *ObjectList::Contains(Object *obj) {
    ObjectEntry *e;
    for (e=head; e!=NULL; e = e->next) {
        if(e->entry->obj == obj)
            return e;
    }
    return NULL;
}
void ObjectList::Append(SceneEntry *e) {
    ObjectEntry *entry = new ObjectEntry(e);
    if(tail)
        tail->next = entry;
    tail = entry;
    if(!head)
        head = entry;
    count++;
}
SceneEnumProc *theSceneEnum = NULL;
ObjectList *theObjects = NULL;
typedef struct {
    Matrix3 *tm;
    Mesh *mesh;
    INode *node;
} MeshAndTMAndNode;
BOOL
UVVertEqual(UVVert tv0, UVVert tv1) {
    return (tv0.x == tv1.x &&
            tv0.y == tv1.y &&
            tv0.y == tv1.y);
}

Mtl*GetMaterial(int materialId,Interface *gi)
{
    int materialCount;
    materialCount = 0;
    
    int materialBaseId;
    
    /*//////////////////////////////////////////////////////////////////////////////
     // The following code includes all materials found in the material library.   //                                                               //
     ////////////////////////////////////////////////////////////////////////////////
     
     // get the material library
     MtlBaseLib& mtlBaseLib = m_pInterface->GetMaterialLibrary();
     
     // loop through all material bases
     for(materialBaseId = 0; materialBaseId < mtlBaseLib.Count(); materialBaseId++)
     {
     // check if it is a material
     if(mtlBaseLib[materialBaseId]->SuperClassID()==MATERIAL_CLASS_ID)
     {
     // get the material
     Mtl *pMtl;
     pMtl = (Mtl *)mtlBaseLib[materialBaseId];
     
     // check if we have a standard material
     if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
     {
     // check if we reached the wanted material
     if(materialId == materialCount)
     {
					// allocate a new max material instance
					CMaxMaterial *pMaxMaterial;
					pMaxMaterial = new CMaxMaterial();
					if(pMaxMaterial == 0)
					{
     theExporter.SetLastError("Memory allocation failed.", __FILE__, __LINE__);
     return 0;
					}
     
					// create the max material
					if(!pMaxMaterial->Create((StdMat *)pMtl))
					{
     delete pMaxMaterial;
     return 0;
					}
     
					return pMaxMaterial;
     }
     
     materialCount++;
     }
     }
     }
     
     /*//////////////////////////////////////////////////////////////////////////////
    
    // get the material bases from the material editor slots
    for(materialBaseId = 0; materialBaseId < 24; materialBaseId++)
    {
        // get material base
        MtlBase *pMtlBase;
        pMtlBase = gi->GetMtlSlot(materialBaseId);
        
        // check if it is a material
        if(pMtlBase->SuperClassID()==MATERIAL_CLASS_ID)
        {
            // get the material
            Mtl *pMtl;
            pMtl = (Mtl *)pMtlBase;
            
            // check if we have a standard material
            if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
            {
                // check if we reached the wanted material
                if(materialId == materialCount)
                {
                    // allocate a new max material instance
                    
                    return pMtl;
                }
                
                materialCount++;
            }
        }
    }
    
    return 0;
    
}




// Add a normal to the list if the smoothing group bits overlap,
// otherwise create a new vertex normal in the list
void VNormal::AddNormal(Point3 &n,DWORD s) {
    if (!(s&smooth) && init) {
        if (next) next->AddNormal(n,s);
        else {
            next = new VNormal(n,s);
        }
    }
    else {
        norm += n;
        smooth |= s;
        init = TRUE;
    }
}

// Retrieves a normal if the smoothing groups overlap or there is
// only one in the list
Point3 &VNormal::GetNormal(DWORD s)
{
    if (smooth & s || !next) return norm;
    else return next->GetNormal(s);
}

// Normalize each normal in the list
void VNormal::Normalize() {
    VNormal *ptr = next, *prev = this;
    while (ptr)
    {
        if (ptr->smooth&smooth) {
            norm += ptr->norm;
            prev->next = ptr->next;
            delete ptr;
            ptr = prev->next;
        }
        else {
            prev = ptr;
            ptr = ptr->next;
        }
    }
    norm = ::Normalize(norm);
    if (next) next->Normalize();
}


void ComputeVertexNormals(Mesh *mesh)
{
    Face *face;
    Point3 *verts;
    Point3 v0, v1, v2;
    Tab<VNormal> vnorms;
    Tab<Point3> fnorms;
    face = mesh->faces;
    verts = mesh->verts;
    vnorms.SetCount(mesh->getNumVerts());
    fnorms.SetCount(mesh->getNumFaces());
    
    // Compute face and vertex surface normals
    for (int i = 0; i < mesh->getNumVerts(); i++) {
        vnorms[i] = VNormal();
    }
    for (int i = 0; i < mesh->getNumFaces(); i++, face++) {
        // Calculate the surface normal
        v0 = verts[face->v[0]];
        v1 = verts[face->v[1]];
        v2 = verts[face->v[2]];
        fnorms[i] = (v1-v0)^(v2-v1);
        for (int j=0; j<3; j++) {
            vnorms[face->v[j]].AddNormal(fnorms[i],face->smGroup);
        }
        fnorms[i] = Normalize(fnorms[i]);
    }
    for (int i=0; i < mesh->getNumVerts(); i++) {
        vnorms[i].Normalize();
    }
    // Display the normals in the debug window of the VC++ IDE
    
    for (int i = 0; i < vnorms.Count(); i++) {
        
    }
    
}




static INT_PTR CALLBACK
ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static XMESHFlags* exp;
    
    switch(message) {
        case WM_INITDIALOG:
            SetWindowContextHelpId(hDlg, idh_3dsexp_export);
            exp = (XMESHFlags*) lParam;
            
            CheckDlgButton(hDlg,IDC_EXPORT_TX, exp->exportTX);
            CheckDlgButton(hDlg,IDC_EXPORT_T,  exp->exportT);
            CheckDlgButton(hDlg,IDC_EXPORT_B,  exp->exportB);
            CheckDlgButton(hDlg,IDC_EXPORT_N,  exp->exportN);
            CheckDlgButton(hDlg,IDC_EXPORT_C,  exp->exportC);
            CheckDlgButton(hDlg,IDC_GEN_A,     exp->generateA);
            CheckDlgButton(hDlg,IDC_OBJ_AUTO_COLOR,     exp->autoFillTXC);
            CheckDlgButton(hDlg,IDC_OBJ_EXPORT_ANIMATION,     exp->exportAnimation);
            SetWindowText(hDlg,exp->nameMesh.c_str());
            
            CenterWindow(hDlg,GetParent(hDlg));
            SetFocus(hDlg); // For some reason this was necessary.  DS-3/4/96
            //CheckDlgButton(hDlg, IDC_MAX_UVS, TRUE);
            return FALSE;
        case WM_DESTROY:
            return FALSE;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDOK:
                    
                    exp->exportTX = IsDlgButtonChecked(hDlg, IDC_EXPORT_TX);
                    exp->exportT  = IsDlgButtonChecked(hDlg, IDC_EXPORT_T);
                    exp->exportB  = IsDlgButtonChecked(hDlg, IDC_EXPORT_B);
                    exp->exportN  = IsDlgButtonChecked(hDlg, IDC_EXPORT_N);
                    exp->exportC  = IsDlgButtonChecked(hDlg, IDC_EXPORT_C);
                    
                    exp->generateA = IsDlgButtonChecked(hDlg, IDC_GEN_A);
                    exp->autoFillTXC = IsDlgButtonChecked(hDlg, IDC_OBJ_AUTO_COLOR);
                    exp->exportAnimation = IsDlgButtonChecked(hDlg, IDC_OBJ_EXPORT_ANIMATION);
                    
                    EndDialog(hDlg, 1);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_CONTEXTHELP) {
                MaxSDK::IHelpSystem::GetInstance()->ShowProductHelpForTopic(idh_3dsexp_export);
                return FALSE;
            }
    }
    return FALSE;
}



static INT_PTR CALLBACK
ExportOptionsAnimationsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static XMESHFlags* exp;
    
    switch(message) {
        case WM_INITDIALOG:
            SetWindowContextHelpId(hDlg, idh_3dsexp_export);
            exp = (XMESHFlags*) lParam;
            CenterWindow(hDlg,GetParent(hDlg));
            SetFocus(hDlg); // For some reason this was necessary.  DS-3/4/96
            //CheckDlgButton(hDlg, IDC_MAX_UVS, TRUE);
            return FALSE;
        case WM_DESTROY:
            return FALSE;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDOK:
                    
                    // for Animation ))
                    
                    
                    EndDialog(hDlg, 1);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_CONTEXTHELP) {
                MaxSDK::IHelpSystem::GetInstance()->ShowProductHelpForTopic(idh_3dsexp_export);
                return FALSE;
            }
    }
    return FALSE;
}







void xtechexp::makeValidURIFilename(TSTR& fn, bool stripMapPaths)
{
    // massage external filenames into valid URI: strip any prefix matching any declared map path,
    //
    /*map \ -> /, sp -> _, : -> $
     if (stripMapPaths) {
     int matchLen = 0, matchI;
     for (int i = 0; i < TheManager->GetMapDirCount(); i++) {
     TSTR dir = TheManager->GetMapDir(i);
     if (MatchPattern(fn, dir + TSTR("*"))) {
     if (dir.length() > matchLen) {
					matchLen = dir.length();
					matchI = i;
     }
     }
     }
     if (matchLen > 0) {
     // found map path prefix, strip it
     TSTR dir = TheManager->GetMapDir(matchI);
     fn.remove(0, dir.length());
     if (fn[0] = _T('\\')) fn.remove(0, 1); // strip any dangling path-sep
     }
     }
     */
    
    // map funny chars
    for (int i = 0; i < fn.length(); i++) {
        if (fn[i] == _T(':')) fn.dataForWrite()[i] = _T('$');
        else if (fn[i] == _T(' ')) fn.dataForWrite()[i] = _T('_');
        else if (fn[i] == _T('\\')) fn.dataForWrite()[i] = _T('/');
    }
}





// new Export with IGAME !

DWORD WINAPI fn(LPVOID arg)
{
    return(0);
}


TCHAR* mapSlotNames[] = {
    _T("Diffuse"),
    _T("Ambient"),
    _T("Specular"),
    _T("SpecularLevel"),
    _T("Opacity"),
    _T("Glossiness"),
    _T("SelfIllumination"),
    _T("Filter"),
    _T("Bump"),
    _T("Reflection"),
    _T("Refraction"),
    _T("Displacement"),
    _T("Unknown") };


void xtechexp::DumpProperty(TCHAR* propName, IGameProperty * prop) {
    
    TSTR Buf;
    IGameKeyTab keys;
    
    if(!prop)	//fix me NH...
        return;
    
    if(prop->GetType() == IGAME_POINT3_PROP)
    {
        Point3 p;
        prop->GetPropertyValue(p);
        Buf.printf(_T("%f %f %f"),p.x,p.y,p.z);
    }
    else if( prop->GetType() == IGAME_FLOAT_PROP)
    {
        float f;
        prop->GetPropertyValue(f);
        Buf.printf(_T("%f"), f);
    }
    else if(prop->GetType()==IGAME_STRING_PROP)
    {
        const TCHAR * b;
        prop->GetPropertyValue(b);
        Buf.printf(_T("$s"),b);
    }
    else
    {
        int i;
        prop->GetPropertyValue(i);
        Buf.printf(_T("%d"), i);
        
    }
    
    MessageBox(GetActiveWindow(), Buf, Buf, MB_OK);
}

#include "md5.h"
#include <string>

I2dAtlas::I2dNode * xtechexp::addBitmapToAtlas(TSTR pathImage, Interface* ip) {
    
    
    std::wstring ws( pathImage.data() );
    std::string strPath(ws.begin(),ws.end());
    std::string hashPath = md5(strPath);
    
    I2dAtlas::I2dNode * position;
    
    
    if (atlasInfoMap.count(hashPath) == 0) {
        // not have in atlas this bitmap ! need add !
        
        // get bitmap from path
        BitmapInfo*info = new BitmapInfo(pathImage);
        Bitmap *bmap = TheManager->Load(info);
        int widthImage  = info->Width();
        int heightImage = info->Height();
        
        
        float scaleX = 1.0;
        float scaleY = 1.0;
        if (widthImage > MAX_SIZE_BITMAP) {
            scaleX = (float)MAX_SIZE_BITMAP / (float)widthImage;
        }
        if (heightImage > MAX_SIZE_BITMAP) {
            scaleY = (float)MAX_SIZE_BITMAP / (float)heightImage;
        }
        
        if (scaleX != 1.0 || scaleY != 1.0) {
            if (scaleX < scaleY) {
                widthImage  = (float)widthImage * scaleX;
                heightImage = (float)heightImage * scaleX;
            }else {
                widthImage  = (float)widthImage * scaleY;
                heightImage = (float)heightImage * scaleY;
            }
        }
        
        
        
        // here i have resized sie for image ! need create some one )
        
        // copy original bitmap info
        BitmapInfo infoReady;
        
        infoReady.SetType(BMM_TRUE_32);
        infoReady.SetWidth(widthImage);
        infoReady.SetHeight(heightImage);
        
        Bitmap *bmapReady = TheManager->Create(&infoReady);
        
        if ( bmapReady == NULL  || bmap == NULL)
        {
            MessageBox(ip->GetMAXHWnd(), _T("Error Create bitmap."), _T("Error"), MB_ICONSTOP);
            return NULL;
        }
        bmapReady->CopyImage(bmap,COPY_IMAGE_RESIZE_HI_QUALITY,0);
        
        
        // fuh here i have resized pic data !
        // okey lets try get pixel data !
        
        //atlasInfo atlasBM atlas
        
        I2dImg*img = (I2dImg*)malloc(sizeof(I2dImg));
        img->width  = widthImage+6;
        img->height = heightImage+6;
        img->rotated = false;
        position = atlas->Insert(*img);
        position->rotated = img->rotated;
        position->rc.width = img->rotated ? heightImage : widthImage;
        position->rc.height = img->rotated ? widthImage : heightImage;
        
        if (!position) {
            MessageBox(ip->GetMAXHWnd(), _T("Error put image to atlas"), _T("Error"), MB_ICONSTOP);
            return NULL;
        }
        
        
        
        BMM_Color_64 *line = ( BMM_Color_64 *)calloc(4, sizeof( BMM_Color_64));
        bool rotated = img->rotated;
        
        int offsetX = position->rc.x;
        int offsetY = position->rc.y;
        
        
        
        if (rotated) {
            
            for(int i=0; i<heightImage; i++) {
                for(int j=0; j< widthImage ; j++) {
                    bmapReady->GetPixels(widthImage-1-j,i,4,line);
                    atlasBM->PutPixels(offsetX+i,offsetY+j,4,line);
                }
            }
            
        }else {
            for(int i=0; i<heightImage; i++) {
                for(int j=0; j<widthImage; j++) {
                    bmapReady->GetPixels(j,i,4,line);
                    atlasBM->PutPixels(offsetX+j,offsetY+i,4,line);
                }
            }
        }
        
        atlasInfoMap[hashPath] = position;
    }else {
        position = atlasInfoMap[hashPath];
    }
    
    return position;
}

#include "vector"



Matrix3 GetNodeTM_(IGameNode *pNode, float time)
{
    // initialize matrix with the identity
    Matrix3 tm, tmParent;
    tm.IdentityMatrix();
    
    // only do this for valid nodes
    if(pNode != 0)
    {
        bool isMirrored = false;
        // get the node transformation
        tm = pNode->GetMaxNode()->GetNodeTM(time);
        
        // This code seems to corrupt some models
        
        /*if(!pNode->GetINode()->IsRootNode())
         {
         tmParent = pNode->GetINode()->GetParentTM(SecToTicks(time));
         
         
         // check if the matrix is right handed
         
         if(DotProd( CrossProd( tmParent.GetRow(0).Normalize(),
         tmParent.GetRow(1).Normalize() ).Normalize(),
         tmParent.GetRow(2).Normalize() ) < -0.1)
         {
         isMirrored = true;
         }
         }*/
        
        // make the transformation uniform
	
        tm.NoScale();
        
        AffineParts parts;
        decomp_affine(tm, &parts);
        parts.q.MakeMatrix(tm);
	    

        if(!isMirrored)
            tm.SetRow(3, parts.t);
        else
            tm.SetRow(3, -parts.t);
        
    }
    
    return tm;
}



bool dumpResetScale = true;



bool checkForBipped(IGameNode*node) {
    Control *pControl;
    pControl = node->GetMaxNode()->GetTMController();
    if((pControl->ClassID() == Class_ID(0x9154,0) ) || (pControl->ClassID() == Class_ID(0x9156,0) )) return true;
    return false;
}



int xtechexp::DumpAnim(IGameObject*obj,IGameNode*node, Interface* ip,std::vector<std::string>*names,std::string*paletText) {
    Skeleton skeleton;
    skeleton.Build(obj);
    
    if(!obj->InitializeData()) {
        return 0;
    }
    

	std::vector<morphChannel*> morpChanels;
    bool exportMorph = false;
    bool skinning = false;
    xmeshFlags.exportSkin = 0;
    int numModifiers = obj->GetNumModifiers();
    for (int i = 0;i < numModifiers;i++) {
        IGameModifier * m = obj->GetIGameModifier(i);
        if(m->IsSkin()) {
            skinning = true;
            xmeshFlags.exportSkin = 1;
        }else if (m->IsMorpher()) {
            
            Modifier *p3dsMaxModifier = m->GetMaxModifier();
            
            if (p3dsMaxModifier->ClassID() == MR3_CLASS_ID) {
                // Get the morpher interface
                MorphR3 *pMorphR3 = static_cast<MorphR3*>(p3dsMaxModifier);
                
                // Gain access to the animation controller of a morph channel
                for (int nChannel=0; nChannel<MR3_NUM_CHANNELS; nChannel++) {
                    morphChannel *pChannel = &pMorphR3->chanBank[nChannel];
                    if (pChannel && pChannel->mActive) {
                        const wchar_t *Name = pChannel->mName.data();
                        morpChanels.push_back(pChannel);
						exportMorph = true;
                    }
                    
                }
            }
        }
    }
    bool frameAnimation = true;
    
    std::string nameMesh = ToStdStr(node->GetName());
    
    names->push_back(nameMesh);
    

	// free data 
	for (uint i = 0; i < 20; i++) {
		float free = 0;
		fwrite(&free, 1, sizeof(float) , stream); // version
	}

    int nameLen = nameMesh.length();
    fwrite(&nameLen,  1, sizeof(int), stream);                 // write len name
    fwrite(nameMesh.c_str(), 1, sizeof(char)*nameLen, stream); // write name
    
    
    
    
    fwrite(&skinning,       1, sizeof(char), stream); //
    fwrite(&frameAnimation, 1, sizeof(char), stream); //
	fwrite(&exportMorph,    1, sizeof(char), stream); //
    
    int i, j, time;
    
    int animStart = (int)( GetCOREInterface()->GetAnimRange().Start() / GetTicksPerFrame() );
    int animEnd   = (int)( GetCOREInterface()->GetAnimRange().End() / GetTicksPerFrame() );
    
    const int numAnimFrames = ( animEnd - animStart ) + 1;
    fwrite(&numAnimFrames, 1, sizeof(int), stream);
    
    
    IGameNode *obj_node = node;
    
    
    if (skinning) {
        
        const int numBones = skeleton.GetNumBones();
        fwrite(&numBones, 1, sizeof(int), stream);
        IGameNode* node;
        Bone* bone, * parent;
        
        for ( i = 0; i < numAnimFrames; ++i )
        {
            time = i * GetTicksPerFrame();
            
            for ( j = 0; j < numBones; ++j )
            {
                bone = skeleton.GetRawBone( j );
                node = bone->GetMaxNode();
                
                GMatrix transform = node->GetWorldTM( time );
                /*
                 if ( bone->GetParentID() != -1 )
                 {
                 parent = bone->GetParent();
                 node = parent->GetMaxNode();
                 GMatrix parentTM = node->GetWorldTM( time );
                 
                 parentTM = parentTM.Inverse();
                 
                 transform *= parentTM;
                 }
                 */
                // write bone
                
                Point3 offset = transform.Translation();
                Quat rotation = transform.Rotation();
                
                // new result
                //if (!checkForBipped(node)) {
                Matrix3 maxNode_transform = GetNodeTM_(node,time);
                
                if ( bone->GetParentID() != -1 )	{
                    parent = bone->GetParent();
                    node   = parent->GetMaxNode();
                    Matrix3 maxNode_parentTM = Inverse(GetNodeTM_(node,time));
                    maxNode_transform=maxNode_transform*maxNode_parentTM;
                }
                
                maxNode_transform = ConvertMax2Ogl(maxNode_transform);
                Quat quat(maxNode_transform);
                rotation.x = quat.x;
                rotation.y = quat.y;
                rotation.z = quat.z;
                rotation.w = quat.w;
                
                offset = maxNode_transform.GetTrans();
                //}
                
                
                int parentID = bone->GetParentID();
                fwrite(&parentID,  1, sizeof(int), stream);
                
                fwrite(&offset.x,  1, sizeof(float), stream); // C
                fwrite(&offset.y,  1, sizeof(float), stream); // C
                fwrite(&offset.z,  1, sizeof(float), stream); // C
                
                fwrite(&rotation.x,  1, sizeof(float), stream); // C
                fwrite(&rotation.y,  1, sizeof(float), stream); // C
                fwrite(&rotation.z,  1, sizeof(float), stream); // C
                fwrite(&rotation.w,  1, sizeof(float), stream); // C
            }
        }
    }
    
    
    
    
    if (frameAnimation) {
        
        
        for ( i = 0; i < numAnimFrames; ++i )
        {
            time = i * GetTicksPerFrame();
            
            GMatrix worldPosition;
            worldPosition = node->GetWorldTM(0).Inverse()*node->GetWorldTM(time);
            
            for( int i = 0; i < 4; ++i )
            {
                Point4 currentPoint = worldPosition.GetRow(i);
                fwrite(&currentPoint[0],  1, sizeof(float), stream); // C
                fwrite(&currentPoint[1],  1, sizeof(float), stream); // C
                fwrite(&currentPoint[2],  1, sizeof(float), stream); // C
                fwrite(&currentPoint[3],  1, sizeof(float), stream); // C
            }
        }
    }
    

	if (exportMorph) {

		int numChannels = morpChanels.size();
		fwrite(&numChannels,  1, sizeof(int), stream); // C

		for ( i = 0; i < numAnimFrames; ++i )
        {
		
            time = i * GetTicksPerFrame();
			
			
			for (int chanelI = 0; chanelI < morpChanels.size(); chanelI++) {
				Interval valid=FOREVER;
				float weight;
				morphChannel*channel = morpChanels[chanelI];
        		channel->cblock->GetValue(0, time, weight, valid);
				fwrite(&weight,  1, sizeof(float), stream); // C
			}

		}
		
	}
    
    return 1;
}


bool vertexCompare(MeshVertex*v,MeshVertex*v2,XMESHFlags*xFl ) {
    bool ret = true;
    if (xFl->exportC || true) {
        ret = ret && (v->GetColor().x == v2->GetColor().x) && (v->GetColor().y == v2->GetColor().y) && (v->GetColor().z == v2->GetColor().z);
    }
    if (xFl->exportN) {
        ret = ret && (v->GetNormal().x == v2->GetNormal().x) && (v->GetNormal().y == v2->GetNormal().y) && (v->GetNormal().z == v2->GetNormal().z);
    }
    if (xFl->exportTX || true) {
        ret = ret && (v->GetTexCoord().x == v2->GetTexCoord().x) && (v->GetTexCoord().y == v2->GetTexCoord().y);
    }
    if (xFl->exportSkin) {
        
        float weights[MAX_BONES_ON_VERTEX], weights2[MAX_BONES_ON_VERTEX];
        float boneIDs[MAX_BONES_ON_VERTEX], boneIDs2[MAX_BONES_ON_VERTEX];
        for (int wi = 0; wi < MAX_BONES_ON_VERTEX; wi++) {
            weights[wi] = 0.0;weights2[wi] = 0.0;
            boneIDs[wi] = 0.0;boneIDs2[wi] = 0.0;
        }
        
        for (int wi = 0; wi < v->GetNumWeights() && wi < MAX_BONES_ON_VERTEX; wi++) {
            weights[wi] = v->GetWeight(wi).GetWeight();
            boneIDs[wi] = v->GetWeight(wi).GetBoneID();
        }
        
        for (int wi = 0; wi < v2->GetNumWeights() && wi < MAX_BONES_ON_VERTEX; wi++) {
            weights2[wi] = v2->GetWeight(wi).GetWeight();
            boneIDs2[wi] = v2->GetWeight(wi).GetBoneID();
        }
        
        ret = ret && (weights[0] ==weights2[0]) && (weights[1] == weights2[1]) && (weights[2] == weights2[2]) && (weights[3] == weights2[3]);
        ret = ret && (boneIDs[0] ==boneIDs2[0]) && (boneIDs[1] == boneIDs2[1]) && (boneIDs[2] == boneIDs2[2]) && (boneIDs[3] == boneIDs2[3]);
    }
    

	ret = ret && (v->getMorphed() == v2->getMorphed());

	std::vector<VertexMorph>& morph1 = v->getMorphing();
	std::vector<VertexMorph>& morph2 = v2->getMorphing();

	if (morph1.size() == morph2.size()) {
		for (int mi = 0;mi < morph1.size(); mi++) {
			ret = ret && (morph1[mi] == morph2[mi]);
		}
	}

	ret = ret && (morph1.size() == morph2.size());

    return ret;
}

std::string printMatrix(float*array) {
    int rowID = 0;
    std::string ret = "";
    for (int row=0; row<4; row++)
    {
        for(int columns=0; columns<4; columns++) {
            char logBuf[256];
            sprintf(logBuf,"%f     ", array[rowID]);
            ret+=logBuf;
            rowID++;
        }
        char logBuf[256];
        sprintf(logBuf,"\n");
        ret+=logBuf;
    }
    return ret;
}
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
int xtechexp::DumpMesh(IGameObject*obj,IGameNode*node, Interface* ip,std::vector<std::string>*names,std::string*paletText) {
    
    std::stringstream sslog;
    std::ofstream oslog;
    // SKIN
    bool skining = false;
    IGameSkin*                skin = NULL;
    Skeleton                  skeleton;
    
    //MESH
    GMatrix					  m_Matrix;
    
    BitArray                  vWritten;
    MaxSDK::Array<MeshVertex> verts;
    Tab<Face3ds>              faces;
    Face3ds                   tmpFace;
    
    xmeshFlags.exportSkin = 0;
    
    
    
    if ( !obj || !node )
        return 0;
    if ( obj->GetIGameType() != IGameMesh::IGAME_MESH )
        return 0;
    
    
    
    
    IGameMesh* mesh		  = static_cast<IGameMesh *>(obj);
    mesh->SetCreateOptimizedNormalList();
    
    if(!obj->InitializeData()) {
        return 0;
    }
    
	bool exportMorph = false;

	morphChannel *pChannelTest = NULL;
	std::vector<morphChannel *> morpChanels;
	int numModifiers = obj->GetNumModifiers();
	for (int i = 0; i < numModifiers; i++) {
		IGameModifier * m = obj->GetIGameModifier(i);
		if (m->IsSkin()) {
			skin = obj->GetIGameSkin();
			if (skin && IGameSkin::IGAME_SKIN == skin->GetSkinType()) {
				xmeshFlags.exportSkin = 1;
				skining = true;
			}
		}
		else if (m->IsMorpher()) {

			Modifier *p3dsMaxModifier = m->GetMaxModifier();

			if (p3dsMaxModifier->ClassID() == MR3_CLASS_ID) {
				// Get the morpher interface
				MorphR3 *pMorphR3 = static_cast<MorphR3*>(p3dsMaxModifier);

				// Gain access to the animation controller of a morph channel
				for (int nChannel = 0; nChannel < MR3_NUM_CHANNELS; nChannel++) {
					morphChannel *pChannel = &pMorphR3->chanBank[nChannel];
					if (pChannel && pChannel->mActive) {
						const wchar_t *Name = pChannel->mName.data();
						morpChanels.push_back(pChannel);
						exportMorph = true;
					}

				}
			}
		}
	}

    
    std::string nameMesh = ToStdStr(node->GetName());
    
    const int faceCount	  = mesh->GetNumberOfFaces();
    const int vertexCount = mesh->GetNumberOfVerts();
    Tab<int> texMaps	  = mesh->GetActiveMapChannelNum();
    
    bool haveDifuseTexture = false;
    try {
        Mtl *m = node->GetMaxNode()->GetMtl();
        Texmap *tmap = m->GetSubTexmap(ID_DI);
        if ((!tmap)||(tmap->ClassID() != Class_ID(BMTEX_CLASS_ID, 0))) { } else {haveDifuseTexture = true;}
    }catch(...){}
    
    
    
    verts.setLengthUsed(vertexCount);
    vWritten.SetSize(vertexCount);
    vWritten.ClearAll();
    
    faces.SetCount(faceCount);
    
    m_Matrix = node->GetObjectTM( 0 );
    
    if (xmeshFlags.exportSkin) {
        skeleton.Build(obj);
    }
    
    xmeshFlags.nameMesh = s2ws(nameMesh);
    
    
    int result = DialogBoxParam(hInstance,
                                MAKEINTRESOURCE(IDD_PANEL),
                                ip->GetMAXHWnd(),
                                ExportOptionsDlgProc,
                                (LPARAM) &xmeshFlags);
    

	if (!xmeshFlags.exportAnimation) {
		exportMorph = false;
		xmeshFlags.exportSkin = 0;
        skining = false;
	}


    if (xmeshFlags.autoFillTXC) {
        xmeshFlags.exportC  = false;
        xmeshFlags.exportTX = false;
    }
	bool objectSpase = !xmeshFlags.exportSkin;
    
    
    Point3 v0, v1, v2;
    Tab<VNormal> vnorms;
    Tab<Point3> fnorms;
    vnorms.SetCount(vertexCount);
    fnorms.SetCount(faceCount);
    
    // Compute face and vertex surface normals
    for (int i = 0; i < vertexCount; i++) {
        vnorms[i] = VNormal();
    }
    for (int i = 0; i < faceCount; i++) {
        
        FaceEx* face = mesh->GetFace( i );
        ;
        // Calculate the surface normal
        v0 = mesh->GetVertex( face->vert[ 0 ], objectSpase);
        v1 = mesh->GetVertex( face->vert[ 1 ], objectSpase);
        v2 = mesh->GetVertex( face->vert[ 2 ], objectSpase);
        fnorms[i] = (v1-v0)^(v2-v1);
        for (int j=0; j<3; j++) {
            vnorms[face->vert[ j ]].AddNormal(fnorms[i],face->smGrp);
        }
        fnorms[i] = Normalize(fnorms[i]);
    }
    for (int i=0; i < mesh->GetMaxMesh()->getNumVerts(); i++) {
        vnorms[i].Normalize();
    }
    
    Matrix3 nodeTM = GetNodeTM_(node,0);
    
    Matrix3 glMatrix;
    
    glMatrix.SetRow(0,Point3(1,0,0));
    glMatrix.SetRow(1,Point3(0,0,-1));
    glMatrix.SetRow(2,Point3(0,1,0));
    glMatrix.SetRow(3,Point3(0,0,0));
    
    

    
    for (int i = 0; i < faceCount; ++i )
    {
        try {
            FaceEx* face = mesh->GetFace( i );
            Point3 color;
            color.x = 1.0;
            color.y = 0.0;
            color.z = 0.0;
            
            bool texcoordNormal = false;
            
            try {
                IGameMaterial * material = mesh->GetMaterialFromFace(face);
                if (material) {
                    IGameProperty* diffuseProp = material->GetDiffuseData();
                    if(diffuseProp->GetType() == IGAME_POINT3_PROP)
                    {
                        diffuseProp->GetPropertyValue(color);
                    }
                    
                    
                    int texCount = material->GetNumberOfTextureMaps();
                    
                    if(texCount!=0) {
                        for(int i=0;i<texCount;i++)
                        {
                            IGameTextureMap * tex = material->GetIGameTextureMap(i);
                            int mapSlot = tex->GetStdMapSlot();
                            if (mapSlot == ID_DI && tex->IsEntitySupported()) {
                                haveDifuseTexture = true;
                                texcoordNormal = true;
                                color.x = 0.0;
                                color.y = 0.0;
                                color.z = 0.0;
                            }
                        }
                    }
                    
                    
                    if (xmeshFlags.autoFillTXC && !texcoordNormal) {
                        xmeshFlags.exportC  = true;
                    }
                    
                    if (xmeshFlags.autoFillTXC && texcoordNormal) {
                        xmeshFlags.exportTX  = true;
                    }
                    
                    /*
                     int texCount = material->GetNumberOfTextureMaps();
                     
                     if(texCount!=0 && xmeshFlags.generateA) {
                     for(int i=0;i<texCount;i++)
                     {
                     IGameTextureMap * tex = material->GetIGameTextureMap(i);
                     int mapSlot = tex->GetStdMapSlot();
                     
                     if (mapSlot == ID_DI && tex->IsEntitySupported()) {
                     // is diffuse )
                     TSTR str = tex->GetBitmapFileName();
                     position = addBitmapToAtlas(str,ip);
                     if (!position) {
                     MessageBox(GetActiveWindow(), L"error get position from atlas", L"fail", MB_OK);
                     }
                     }
                     }
                     }
                     */
                }
            }catch(...){}
            
  
            for (int k = 0; k < 3; k++) {
                tmpFace.vNum[k] = face->vert[k];
            }
            
            tmpFace.flags = face->flags;
            

            
            
            for (int j = 0; j < 3; ++j )
            {
				Point3 p = mesh->GetVertex( face->vert[ j ], objectSpase);
                
                
                MeshVertex v( p.x, p.y, p.z );

				

				Point3 p2 = p;
				for (int chanelI = 0; chanelI < morpChanels.size();chanelI++) {
                    morphChannel* channel = morpChanels[chanelI];
                
					
                    Point3 p1 = channel->mPoints[face->vert[ j ]]; // for test
                    p1 = p1*glMatrix;

					

					Point3 delta = channel->mDeltas[face->vert[j]]*glMatrix;
                    if (delta.x!=0.0 || delta.y != 0.0 || delta.z != 0.0) {
						//delta.x*=100; // persent animate
						//delta.y*=100; // persent animate 
						//delta.z*=100; // persent animate

						v.setMorphed(true);
						p2 = p - delta; // for test 
                    }
					v.addMorphing(VertexMorph(delta));
                }

				//v.setPos(p2.x, p2.y, p2.z);
				


                Point3 n = mesh->GetNormal( face, j );
                
                
                
                v.SetNormal( vnorms[face->vert[ j ]].GetNormal(face->smGrp).x, vnorms[face->vert[ j ]].GetNormal(face->smGrp).y, vnorms[face->vert[ j ]].GetNormal(face->smGrp).z );
                v.SetID( face->vert[ j ] );
                
                Point3 uvw;
                DWORD indices[3];
                
                
                Point2 tVert;
                try {
                    int iTexIndex = mesh->GetFaceTextureVertex(i, j, 1);
                    tVert = mesh->GetTexVertex(iTexIndex);
                    if ( texMaps.Count() <= 0 ) {
                        if ( mesh->GetMapFaceIndex( texMaps[ 0 ], i, indices ) )
                            uvw = mesh->GetMapVertex( texMaps[ 0 ], indices[ j ] );
                        else
                            uvw = mesh->GetMapVertex( texMaps[ 0 ], face->vert[ j ] );
                    }
                }catch(...){
                    // is bad
                }
                
                
                if(texcoordNormal) {
                    v.SetTexCoord( tVert.x, 1.0f - tVert.y );
                }else {
                    v.SetTexCoord( 0.0, 0.0 );
                }
                
                
                if (xmeshFlags.exportSkin) {
                    int vID = face->vert[ j ];
                    int numW = skin->GetNumberOfBones( vID );
                    
                    if ( IGameSkin::IGAME_RIGID == skin->GetVertexType( vID ) ) numW = 1;
                    
                    for (int b = 0; b < numW; ++b )
                    {
                        float w = skin->GetWeight( vID, b );
                        if ( numW == 1 )
                            w = 1.0f;
                        int boneId = skeleton.GetBoneIDByMaxID( skin->GetBoneID( vID, b ) );
                        v.AddWeight( VertexWeight( w, boneId ) );
                    }
                }
                
                v.setColor(color.x,color.y,color.z);
                
                if (vWritten[face->vert[j]]) {
					if (!vertexCompare(&verts[face->vert[j]], &v,&xmeshFlags)) {
                        tmpFace.vNum[j] = (int)verts.length();
                        verts.insertAt(verts.length(), v);
                    }
                } else {
                    verts[face->vert[j]] = v;
                    vWritten.Set(face->vert[j]);
                }
                
            }
            faces[i] = tmpFace;
            
            
        }catch(...) {
            printf("bad");
        }
        
    }

    
	int typeObject = 0;
	// type object  0 -> MESH
	fwrite(&typeObject, 1, sizeof(int), stream);


	// free data 
	for (uint i = 0; i < 20; i++) {
		float free = 0;
		fwrite(&free, 1, sizeof(float) , stream); // version
	}
	
	// position object 
	float EulerX, EulerY, EulerZ;
	Quat quaternion = m_Matrix.Rotation();
	quaternion.GetEuler(&EulerX, &EulerY, &EulerZ);

	Point3 scaling = m_Matrix.Scaling();
	Point3 translate = m_Matrix.Translation();

	fwrite(&quaternion.x, 1, sizeof(float), stream);
	fwrite(&quaternion.y, 1, sizeof(float), stream);
	fwrite(&quaternion.z, 1, sizeof(float), stream);
	fwrite(&quaternion.w, 1, sizeof(float), stream);

	fwrite(&scaling.x, 1, sizeof(float), stream);
	fwrite(&scaling.y, 1, sizeof(float), stream);
	fwrite(&scaling.z, 1, sizeof(float), stream);

	fwrite(&translate.x, 1, sizeof(float), stream);
	fwrite(&translate.y, 1, sizeof(float), stream);
	fwrite(&translate.z, 1, sizeof(float), stream);

	Box3 bbox;
	mesh->GetBoundingBox(bbox);
	Point3 bboxMax = bbox.Max();
	Point3 bboxMin = bbox.Min();
	Point3 bboxCnt = bbox.Center();

	fwrite(&bboxMax.x, 1, sizeof(float), stream);
	fwrite(&bboxMax.y, 1, sizeof(float), stream);
	fwrite(&bboxMax.z, 1, sizeof(float), stream);

	fwrite(&bboxMin.x, 1, sizeof(float), stream);
	fwrite(&bboxMin.y, 1, sizeof(float), stream);
	fwrite(&bboxMin.z, 1, sizeof(float), stream);

	fwrite(&bboxCnt.x, 1, sizeof(float), stream);
	fwrite(&bboxCnt.y, 1, sizeof(float), stream);
	fwrite(&bboxCnt.z, 1, sizeof(float), stream);



    
    names->push_back(nameMesh);
    int nameLen = nameMesh.length();

	// name 
    fwrite(&nameLen,  1, sizeof(int), stream);                 // write len name
    fwrite(nameMesh.c_str(), 1, sizeof(char)*nameLen, stream); // write name
    

    unsigned int sizeVertex = 12; // position size
    
    if (xmeshFlags.exportTX) {
        sizeVertex+=sizeof(float)*2;
    }
    if (xmeshFlags.exportT) {
        sizeVertex+=sizeof(float)*3;
    }
    if (xmeshFlags.exportB) {
        sizeVertex+=sizeof(float)*3;
    }
    if (xmeshFlags.exportN) {
        sizeVertex+=sizeof(float)*3;
    }
    if (xmeshFlags.exportC) {
        sizeVertex+=sizeof(float)*3;
    }
    if (xmeshFlags.exportSkin) {
        sizeVertex+=sizeof(float)*8;
    }
    
    
    unsigned int maxSizeVChunk = 20000000;
    
    //sizeVertex
    
    uint numVertexInChunk = (((maxSizeVChunk/sizeVertex)-1)/3)*3; // %3
    uint sizeChunk = numVertexInChunk*sizeVertex; // size chunk !
    

	
    
    
    std::vector<std::vector<Face3ds >> fasesSeparated;
    std::vector<std::vector<MeshVertex >> vertexSeparated;
    std::vector<Face3ds> f;
    std::vector<MeshVertex> v;
    fasesSeparated.push_back(f);
    vertexSeparated.push_back(v);
    
    unsigned int currentwritedVerts = 0;
    uint chunkIndx = 0;
    
    
    vWritten.SetSize((faces.Count()*3)+1);
    vWritten.ClearAll();
    
    std::vector<int> facesStored;
    facesStored.resize((faces.Count()*3)+1);
    
    for (int fI = 0;fI < faces.Count();fI++) {
        
        int faceX = faces[fI].vNum[0];
        int faceY = faces[fI].vNum[1];
        int faceZ = faces[fI].vNum[2];
        
        MeshVertex vertxA = verts[faceX];
        MeshVertex vertxB = verts[faceY];
        MeshVertex vertxC = verts[faceZ];
        
        
        if (currentwritedVerts+3 >= numVertexInChunk) {
            chunkIndx++;
            currentwritedVerts = 0;
            std::vector<Face3ds> f;
            std::vector<MeshVertex> v;
            fasesSeparated.push_back(f);
            vertexSeparated.push_back(v);
            
            vWritten.ClearAll();
        }
        
        
        uint numVertsCurWrited = 0;
        
        
        if (vWritten[faceX]) {
            faceX = facesStored[faceX];
        }else {
            vWritten.Set(faceX);
            facesStored[faceX] = currentwritedVerts;
            faceX = currentwritedVerts;
            vertexSeparated[chunkIndx].push_back(vertxA);
            currentwritedVerts++;
        }
        
        if (vWritten[faceY]) {
            faceY = facesStored[faceY];
        }else {
            vWritten.Set(faceY);
            facesStored[faceY] = currentwritedVerts;
            faceY = currentwritedVerts;
            vertexSeparated[chunkIndx].push_back(vertxB);
            currentwritedVerts++;
        }
        
        
        if (vWritten[faceZ]) {
            faceZ = facesStored[faceZ];
        }else {
            vWritten.Set(faceZ);
            facesStored[faceZ] = currentwritedVerts;
            faceZ = currentwritedVerts;
            vertexSeparated[chunkIndx].push_back(vertxC);
            currentwritedVerts++;
        }
        
        Face3ds newFace;
        newFace.vNum[0] = faceX;
        newFace.vNum[1] = faceY;
        newFace.vNum[2] = faceZ;
        
        fasesSeparated[chunkIndx].push_back(newFace);
    }
    


	int indexMorhedVertex = 0;
	int indexNotMorpedVertex = verts.length()-1;


	if (exportMorph && chunkIndx != 0) {
		MessageBox(ip->GetMAXHWnd(), _T("Error morphing not exported mesh its to big."), _T("Error"), MB_ICONSTOP); 
		exportMorph = false;
	}


	


    if (exportMorph && chunkIndx == 0) {

		int chunkV = 0;
		std::vector<MeshVertex> vertsN = vertexSeparated[chunkV];
        std::vector<Face3ds> *facesN   = &fasesSeparated[chunkV];

		std::vector<int> buffer;
		buffer.resize(vertsN.size());


		vWritten.SetSize(vertsN.size()+1);
        vWritten.ClearAll();
		

		MaxSDK::Array<MeshVertex> vertsWithMorph;
		vertsWithMorph.setLengthUsed(vertsN.size());

		for (int i = 0; i < facesN->size();i++) {
			Face3ds*face = &(*facesN)[i];
			for (int j = 0; j < 3; j++) {
				

				if (vWritten[face->vNum[j]]) {
					face->vNum[j] = buffer[face->vNum[j]];
				}else {
					MeshVertex vertex = vertsN[face->vNum[j]];
					int prevFaceIndx = face->vNum[j];
					if (vertex.getMorphed()) {
						// morphed 
						vertsWithMorph[indexMorhedVertex] = vertex;
						face->vNum[j] = indexMorhedVertex;
						indexMorhedVertex++;
					}else {
						vertsWithMorph[indexNotMorpedVertex] = vertex;
						face->vNum[j] = indexNotMorpedVertex;
						indexNotMorpedVertex--;
					}
					vWritten.Set(prevFaceIndx);
					buffer[prevFaceIndx] = face->vNum[j];
				}
			}
		}

		vertexSeparated[chunkV].clear();
		vertexSeparated[chunkV].resize(vertsWithMorph.length());
		for (int i = 0; i < vertsWithMorph.length(); i++) {
			vertexSeparated[chunkV][i] = vertsWithMorph[i];
		}
	}

    chunkIndx++;
    


	fwrite(&xmeshFlags.exportTX,    1, sizeof(char), stream); // TX
    fwrite(&xmeshFlags.exportT,     1, sizeof(char), stream); // T
    fwrite(&xmeshFlags.exportB,     1, sizeof(char), stream); // B
    fwrite(&xmeshFlags.exportN,     1, sizeof(char), stream); // N
    fwrite(&xmeshFlags.exportC,     1, sizeof(char), stream); // C
    fwrite(&xmeshFlags.exportSkin,  1, sizeof(char), stream); // C
	fwrite(&exportMorph,            1, sizeof(char), stream); // C version 3+
	fwrite(&xmeshFlags.exportAnimation,            1, sizeof(char), stream); // 

	const int numVerts = verts.length();
	const int numFaces = faces.Count();



    fwrite(&numVerts, 1, sizeof(int), stream);
    fwrite(&numFaces, 1, sizeof(int), stream);

    fwrite(&numVertexInChunk, 1, sizeof(int), stream);

    fwrite(&chunkIndx, 1, sizeof(int), stream);
    
    for (int chunkV = 0; chunkV < chunkIndx; chunkV++){
        std::vector<MeshVertex> vertsN = vertexSeparated[chunkV];
        std::vector<Face3ds> facesN = fasesSeparated[chunkV];
        
        uint sizeChunkVerts = vertsN.size();
        uint sizeChunkFaces = facesN.size();
        
        fwrite(&sizeChunkVerts, 1, sizeof(int), stream);
        fwrite(&sizeChunkFaces, 1, sizeof(int), stream);
        
        for (int vCount = 0; vCount < vertsN.size(); vCount++) {
            MeshVertex v = vertsN[vCount];
            writeVert(v,paletText);
        }
		if (exportMorph) {
			//num morphed vertex 
			fwrite(&indexMorhedVertex, 1, sizeof(int), stream);
			// num morphing channels
			int numChannelsMorph = vertsN[0].getMorphing().size();
			fwrite(&numChannelsMorph, 1, sizeof(int), stream);

			for (int channelID = 0; channelID < numChannelsMorph; channelID++) {
				for (int vCount = 0; vCount < indexMorhedVertex; vCount++) {
            		MeshVertex v = vertsN[vCount];
            		writeVertexMorphInfo(v,channelID);
        		}
			}
		}
		
        
        for (int fCount = 0; fCount < facesN.size(); fCount++) {
            int faceX = facesN[fCount].vNum[0];
            int faceY = facesN[fCount].vNum[1];
            int faceZ = facesN[fCount].vNum[2];
            
            char buf[256];
            sprintf(buf,"\n%d %d %d",faceX,faceY,faceZ);
            (*paletText)+=buf;
            
            fwrite(&faceX, 1, sizeof(int), stream);
            fwrite(&faceY, 1, sizeof(int), stream);
            fwrite(&faceZ, 1, sizeof(int), stream);
        }
    }
    
    
    
    if (xmeshFlags.exportSkin) {
        int numBones = skeleton.m_Bones.size();
        fwrite(&numBones, 1, sizeof(int), stream);
        for (int i = 0; i < numBones;i++){
            Bone* bone = skeleton.GetRawBone( i );
            node = bone->GetMaxNode();
            
            GMatrix transform = node->GetWorldTM( 0 );
            transform = transform.Inverse();
            Point3 offset = transform.Translation();
            Quat rotation = transform.Rotation();
            
            //if (!checkForBipped(node)) {
            Matrix3 n = Inverse(GetNodeTM_(node,0));


            n = ConvertMax2Ogl(n);
            
            offset = n.GetTrans();
            Quat quat(n);
            rotation.x = quat.x;
            rotation.y = quat.y;
            rotation.z = quat.z;
            rotation.w = quat.w;
            //}
            
            int parentID = bone->GetParentID();
            fwrite(&parentID,  1, sizeof(int), stream);
            // world space
            
            char buf[256];
            sprintf(buf,"\nparentID:%d\noffset %f %f %f",parentID,offset.x,offset.y,offset.z);
            (*paletText)+=buf;
            
            char buf2[256];
            sprintf(buf2,"\nrotation %f %f %f %f",rotation.x,rotation.y,rotation.z,rotation.w);
            (*paletText)+=buf2;
            
            fwrite(&offset.x,  1, sizeof(float), stream); // C
            fwrite(&offset.y,  1, sizeof(float), stream); // C
            fwrite(&offset.z,  1, sizeof(float), stream); // C
            
            fwrite(&rotation.x,  1, sizeof(float), stream); // C
            fwrite(&rotation.y,  1, sizeof(float), stream); // C
            fwrite(&rotation.z,  1, sizeof(float), stream); // C
            fwrite(&rotation.w,  1, sizeof(float), stream); // C
            
        }
    }
    
    return 1;
}


void xtechexp::writeVertexMorphInfo(MeshVertex& vertx,int channelID) {
	
	std::vector<VertexMorph>& morp = vertx.getMorphing();
    VertexMorph& morpInfo = morp[channelID];

	float x = morpInfo.getdelta().x;
	float y = morpInfo.getdelta().y;
	float z = morpInfo.getdelta().z;

	fwrite(&x, 1, sizeof(float), stream);
    fwrite(&y, 1, sizeof(float), stream);
    fwrite(&z, 1, sizeof(float), stream);

}


int xtechexp::writeVert(MeshVertex& vertx,std::string*paletText) {
    
    
    // vertex mirrored for openGL !!!
    fwrite(&vertx.GetPosition().x, 1, sizeof(float), stream);
    fwrite(&vertx.GetPosition().y, 1, sizeof(float), stream);
    fwrite(&vertx.GetPosition().z, 1, sizeof(float), stream);
    
    char buf[256];
    sprintf(buf,"\n%f %f %f",vertx.GetPosition().x,vertx.GetPosition().y,vertx.GetPosition().z);
    (*paletText)+=buf;
    
    
    if (xmeshFlags.exportTX) {
        fwrite(&vertx.GetTexCoord().x, 1, sizeof(float), stream);
        fwrite(&vertx.GetTexCoord().y, 1, sizeof(float), stream);
    }
    
    
    if (xmeshFlags.exportT) {
        // need calculate
        fwrite(0, 1, sizeof(float), stream);
        fwrite(0, 1, sizeof(float), stream);
        fwrite(0, 1, sizeof(float), stream);
    }
    
    
    if (xmeshFlags.exportB) {
        // need calculate
        fwrite(0, 1, sizeof(float), stream);
        fwrite(0, 1, sizeof(float), stream);
        fwrite(0, 1, sizeof(float), stream);
        
    }
    
    
    if (xmeshFlags.exportN) {
        
        
        
        fwrite(&vertx.GetNormal().x, 1, sizeof(float), stream);
        fwrite(&vertx.GetNormal().y, 1, sizeof(float), stream);
        fwrite(&vertx.GetNormal().z, 1, sizeof(float), stream);
    }
    
    
    if (xmeshFlags.exportC) {
        
        fwrite(&vertx.GetColor().x, 1, sizeof(float), stream);
        fwrite(&vertx.GetColor().y, 1, sizeof(float), stream);
        fwrite(&vertx.GetColor().z, 1, sizeof(float), stream);
    }
    if (xmeshFlags.exportSkin) {
        float weights[MAX_BONES_ON_VERTEX];
        float boneIDs[MAX_BONES_ON_VERTEX];
        for (int wi = 0; wi < MAX_BONES_ON_VERTEX; wi++) {
            weights[wi] = 0.0;
            boneIDs[wi] = 0.0;
        }
        for (int wi = 0; wi < vertx.GetNumWeights() && wi < MAX_BONES_ON_VERTEX; wi++) {
            weights[wi] = vertx.GetWeight(wi).GetBoneID() < 0 ? 0.0 : vertx.GetWeight(wi).GetWeight();
            boneIDs[wi] = vertx.GetWeight(wi).GetBoneID() < 0 ? 0.0 : vertx.GetWeight(wi).GetBoneID();
        }
        
        
        char buf2[512];
        sprintf(buf2,"\nboneID %f weight%f\nboneID %f weight%f\nboneID %f weight%f\nboneID %f weight%f\n",
                boneIDs[0],weights[0],
                boneIDs[1],weights[1],
                boneIDs[2],weights[2],
                boneIDs[3],weights[3]);
        (*paletText)+=buf2;
        
        
        
        fwrite(&weights[0], 1, sizeof(float), stream);
        fwrite(&weights[1], 1, sizeof(float), stream);
        fwrite(&weights[2], 1, sizeof(float), stream);
        fwrite(&weights[3], 1, sizeof(float), stream);
        
        fwrite(&boneIDs[0], 1, sizeof(float), stream);
        fwrite(&boneIDs[1], 1, sizeof(float), stream);
        fwrite(&boneIDs[2], 1, sizeof(float), stream);
        fwrite(&boneIDs[3], 1, sizeof(float), stream);
    }
    return 1;
}


void xtechexp::enumMeshs(IGameNode*child,int*numMeshs) {
	Interval valid;


    IGameObject * obj = child->GetIGameObject();
    int typeObject = obj->GetIGameType();
    switch(typeObject){
        case IGameObject::IGAME_MESH:
            (*numMeshs)++;
            break;
		case IGameObject::IGAME_CAMERA:
			/*
			ObjectEntry* oe = (ObjectEntry*)obj;

			if (!oe->entry) break;
			if (!oe->entry->node) break;

			INode*node = oe->entry->node;

			CameraObject* camobj = (CameraObject *)oe->entry->obj;
			GenCamera *gc = (GenCamera*)oe->entry->obj;
			CameraState cs;
			camobj->EvalCameraState(theSceneEnum->time, valid, &cs);
		
			Matrix3 mat = node->GetNodeTM(theSceneEnum->time);
			Point3 pos = mat.GetRow(3);
			*/

            break;
    }
    
    for(int i=0;i<child->GetChildCount();i++)
    {
        IGameNode * newchild = child->GetNodeChild(i);
        
        // we deal with targets in the light/camera section
        if(newchild->IsTarget())
            continue;
        
        enumMeshs(newchild,numMeshs);
    }
}


int xtechexp::ExportChildNodeInfo(IGameNode*child, Interface* ip,std::vector<long>*offsets,std::vector<std::string>*names,std::string*paletText, std::vector<long>*sizeMeshs) {
    
    IGameObject * obj = child->GetIGameObject();
    int typeObject = obj->GetIGameType();
    switch(typeObject){
        case IGameObject::IGAME_MESH:
			long offsetStart = ftell(stream);
            offsets->push_back(offsetStart);
            DumpMesh(obj,child,ip,names,paletText);
			long size = ftell(stream) - offsetStart;
			sizeMeshs->push_back(size);
			if (xmeshFlags.exportAnimation) {
				DumpAnim(obj, child, ip, names, paletText);
			}
        break;
    }
    
    
    for(int i=0;i < child->GetChildCount();i++)
    {
        IGameNode * newchild = child->GetNodeChild(i);
        if(newchild->IsTarget())
            continue;
        
        ExportChildNodeInfo(newchild,ip,offsets,names,paletText, sizeMeshs);
    }
    
    return 0;
}

template<class T>
T base_name(T const & path, T const & delims = "/\\")
{
    return path.substr(path.find_last_of(delims) + 1);
}
template<class T>
T remove_extension(T const & filename)
{
    typename T::size_type const p(filename.find_last_of('.'));
    return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

#include "pugixml.hpp"


int	xtechexp::DoExport(const TCHAR* filename, ExpInterface* ei, Interface* ip, BOOL suppressPrompts, DWORD /*options*/)
{
    
    float igameVersion  = GetIGameVersion();
    
    std::string fileNameS = ToStdStr(filename);
    
    xmeshFlags.exportTX = 0;
    xmeshFlags.exportT = 0;
    xmeshFlags.exportB = 0;
    xmeshFlags.exportN = 1;
    xmeshFlags.exportC = 0;
    xmeshFlags.generateA = 0;
    xmeshFlags.autoFillTXC = 1;
    xmeshFlags.exportSkin = 0;
    xmeshFlags.exportAnimation = 0;


    ip->ProgressStart(_T("Exporting XMESH"), TRUE, fn, NULL);
    
    pIgame = GetIGameInterface();
    
    IGameConversionManager* cm = GetConversionManager();
    cm->SetCoordSystem( IGameConversionManager::IGAME_OGL );
    
    pIgame->InitialiseIGame();
    pIgame->SetStaticFrame(0);
    
    bool exportResult = true;// _xtech_save(filename,  ei,  ip,&xmeshFlags);
    
    
    std::string nameModel = remove_extension(base_name(fileNameS));
    
    
    
    int result = DialogBoxParam(hInstance,
                                MAKEINTRESOURCE(IDD_PANEL),
                                ip->GetMAXHWnd(),
                                ExportOptionsDlgProc,
                                (LPARAM) &xmeshFlags);
    
    if (xmeshFlags.generateA) {
        atlas = new I2dAtlas();
        atlas->Size(0, 0, 1024, 1024);
        
        atlasInfo.SetType(BMM_TRUE_32);
        atlasInfo.SetWidth(1024);
        atlasInfo.SetHeight(1024);
        atlasBM = TheManager->Create(&atlasInfo);
        if ( atlasBM == NULL ) { 
            MessageBox(ip->GetMAXHWnd(), _T("Error Create bitmap."), _T("Error"), MB_ICONSTOP); 
        } 
    }
    
    
    if (result <= 0) {
        ip->ProgressEnd();	
        return 0;
    }
    
    WorkFile theFile(filename,_T("wb"));
    stream = theFile.Stream();
    
    if(!stream) {
        Alert(0);	
        ip->ProgressEnd();	
        return(0);
    }
    
    XMESHFlags*xFl = &xmeshFlags;
    
    
    
	float version = 0.2;
    int numMeshes = 0;
	int numLights = 1;
	int numCameras = 1;

	int typeCamera = 0; // free camera
	float cameraView[3] = {0,0,0};
	float cameraPos[3] = { 0,10,-15 };
	float cameraUp[3] = { 0,1,0 };

	int typeLights = 0; // ortho direct Light
	float lightView[3] = { 0,0,0 };
	float lightPos[3] = { 0,10,-15 };
	float lightUp[3] = { 0,1,0 };
	int typeShadow = 0;// NO SHADOW

	float free[20];
	

    for(int loop = 0; loop <pIgame->GetTopLevelNodeCount();loop++)
    {
        IGameNode * pGameNode = pIgame->GetTopLevelNode(loop);
        enumMeshs(pGameNode,&numMeshes);
    }

	 
	fwrite(&version, 1, sizeof(float), stream); // version

	fwrite(&numMeshes, 1, sizeof(int), stream); // num meshes 
	fwrite(&numCameras, 1, sizeof(int), stream); // num cameras
	fwrite(&numLights, 1, sizeof(int), stream); // num lights 

	fwrite(&typeCamera, 1, sizeof(int), stream); // free perspective camera

	fwrite(&cameraPos[0], 1, sizeof(float) * 3, stream); // 
	fwrite(&cameraView[0], 1, sizeof(float) * 3, stream); // 
	fwrite(&cameraUp[0], 1, sizeof(float) * 3, stream); // 
	fwrite(&free, 1, sizeof(float) * 20, stream); // unused


	Quat quaternion(0.9,0.3,0.0,0.0); // Z 45 
	Point3 translate(0,10,10);
	fwrite(&typeLights, 1, sizeof(int), stream); //  direct light

	fwrite(&quaternion.x, 1, sizeof(float), stream);
	fwrite(&quaternion.y, 1, sizeof(float), stream);
	fwrite(&quaternion.z, 1, sizeof(float), stream);
	fwrite(&quaternion.w, 1, sizeof(float), stream);

	fwrite(&translate.x, 1, sizeof(float), stream);
	fwrite(&translate.y, 1, sizeof(float), stream);
	fwrite(&translate.z, 1, sizeof(float), stream);

	fwrite(&typeShadow, 1, sizeof(int), stream); // no shadow
	fwrite(&free, 1, sizeof(float) * 20, stream); // unused

	// free data 
	
	fwrite(&free, 1, sizeof(float)*20, stream); // version


    std::vector<long> offsets;
	std::vector<long> sizeMeshs;
    std::vector<std::string> names;

	std::string paletText = ""; // need remove !

    for(int loop = 0; loop <pIgame->GetTopLevelNodeCount();loop++)
    {
        IGameNode * pGameNode = pIgame->GetTopLevelNode(loop);
        ExportChildNodeInfo(pGameNode,ip,&offsets,&names,&paletText,&sizeMeshs);
    }

    pIgame->ReleaseIGame();
    
    uint sizeCurrent = sizeof(long);
    for (long i = 0; i < offsets.size();i++) {
        long offset = offsets[i];
		long size = sizeMeshs[i];
        fwrite(&offset,  1, sizeof(long), stream); // save offsets ! 
		fwrite(&size, 1, sizeof(long), stream); // save size mesh data ! 
    }
    


    
    if (xmeshFlags.generateA) {
        // save atlas info ! 
        BOOL gotIt = TheManager->SelectFileOutput(&atlasInfo, ip->GetMAXHWnd());
        if (gotIt) {
            atlasBM->OpenOutput(& atlasInfo);
            atlasBM->Write(&atlasInfo);
            atlasBM->Close(&atlasInfo);
            atlasBM->Display(L"The copied Bitmap");	
        }
    }
    
    ip->ProgressEnd();	
    return exportResult;
}


