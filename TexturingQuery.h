#ifndef TEXTURINGQUERY_H
#define TEXTURINGQUERY_H
#include <string>
#include <spatialindex/SpatialIndex.h>
#include <osg/Geode>
#include <osg/BoundingBox>
#include <osg/Geometry>
#include "TexPyrAtlas.h"
#include <libsnapper/auv_camera_calib.hpp>
using libsnapper::Camera_Calib;
class ObjVisitor : public SpatialIndex::IVisitor
{
private:
    uint32_t m_indexIO;
    uint32_t m_leafIO;
    std::vector<SpatialIndex::id_type> m_vector;
    uint32_t nResults;

public:

    ObjVisitor();
    ~ObjVisitor();

    uint32_t GetResultCount() const { return nResults; }
    std::vector<SpatialIndex::id_type>& GetResults()  { return m_vector; }

    void visitNode(const SpatialIndex::INode& n);
    void visitData(const SpatialIndex::IData& d);
    void visitData(std::vector<const SpatialIndex::IData*>& v);
};
class MyDataStream;
typedef std::vector<std::pair<SpatialIndex::id_type,double> > CamDists;

class TexturingQuery
{
public:
    TexturingQuery(std::string bbox_file,Camera_Calib &calib);
    ~TexturingQuery();
    void projectModel(osg::Geode *,int texSizeIdx);
    class ProjectionCamera{
    public:
        osg::Matrixf m;
        std::string filename;
        long id;
        osg::BoundingBox bb;
    };
    typedef std::map<SpatialIndex::id_type,ProjectionCamera>  CameraVector;
    struct sort_pred {
        bool operator()(const std::pair<SpatialIndex::id_type,double> &left, const std::pair<SpatialIndex::id_type,double> &right) {
            return left.second < right.second;
        }
    };

protected:
    typedef std::multimap<int,ProjectionCamera> ProjectsToMap;
    double getDistToCenter(osg::Vec3 v, ProjectionCamera cam);
    const CamDists getClosest(std::vector<int> tri_v,const osg::Vec3Array &verts);
    std::string _bbox_file;
    Camera_Calib _calib;

    SpatialIndex::ISpatialIndex* tree;
    SpatialIndex::StorageManager::IBuffer* file;
    SpatialIndex::IStorageManager* diskfile;
    std::string baseName;
    double utilization;
    int capacity;
    MyDataStream *stream;
    osg::Vec4Array* projectAllTriangles(const osg::PrimitiveSet& prset, const osg::Vec3Array &verts);
    osg::StateSet *generateStateAndAtlasRemap( osg::Vec4Array *v, int texSizeIdx);
    static const int TEXUNIT_ARRAY=0;
    ProjectsToMap reproj;

    typedef std::pair<unsigned int, std::string> AttributeAlias;
    void setVertexAttrib(osg::Geometry& geom, const AttributeAlias& alias, osg::Array* array, bool normalize, osg::Geometry::AttributeBinding binding);
    std::vector<osg::ref_ptr<osg::Image> >loadTex(std::map<SpatialIndex::id_type,int> allIds,int sizeIdx);
    osg::Vec2 reprojectPt(const osg::Matrixf &mat,const osg::Vec3 &v);
    AttributeAlias _vertexAlias;
    AttributeAlias _projCoordAlias;
    TexPyrAtlas *_atlasGen;
    CameraVector _cameras;


};

#endif // TEXTURINGQUERY_H