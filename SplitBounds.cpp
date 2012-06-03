#include "SplitBounds.h"
#include "MemUtils.h"
#include "ShellCmd.h"
using namespace std;
osg::Matrix  osgTranspose( const osg::Matrix& src )
{
    osg::Matrix dest;
    for( int i = 0; i < 4; i++ )
        for( int j = 0; j < 4; j++ )
            dest(i,j) = src(j,i);
    return dest;
}

WriteBoundTP::WriteBoundTP(double res,string fname,std::string basepath,const std::vector<Stereo_Pose_Data> &tasks):WriteTP(res,fname,basepath){
    cmdfp =fopen(fname.c_str(),"w");
    if(!cmdfp){
        fprintf(stderr,"Can't create cmd file");
        exit(-1);
    }
    char tmp[1024];
    sprintf(tmp,"%s/vcglist.txt",aggdir);
    bboxfn=string(tmp);
    FILE *fp=fopen(bboxfn.c_str(),"w");
    for(int i=0; i< (int)tasks.size(); i++)
        fprintf(fp,"%s/%s %f %f %f %f %f %f\n",aggdir,tasks[i].mesh_name.c_str(),
                tasks[i].bbox._min[0],
                tasks[i].bbox._min[1],
                tasks[i].bbox._min[2],
                tasks[i].bbox._max[0],
                tasks[i].bbox._max[1],
                tasks[i].bbox._max[2]
                );
    fclose(fp);

}
bool WriteBoundTP::write_cmd(Cell_Data<Stereo_Pose_Data> cell){
 double smallCCPer=0.2;
    fprintf(cmdfp,"%s/vcgapps/bin/plymc -M -V%f -s %d %d %d %d %d %d -o%s/vol %s;%s/vcgapps/bin/mergeMesh %s/vol_%d%d%d.ply  -cleansize %f -P -out %s/clean_%d%d%d.ply\n",_basepath.c_str(),
            _res,
            cell.splits[0],
            cell.splits[1],
            cell.splits[2],
            cell.volIdx[0],
            cell.volIdx[1],
            cell.volIdx[2],
            aggdir,
            bboxfn.c_str(),
            _basepath.c_str(),
            aggdir,
            cell.volIdx[0],
            cell.volIdx[1],
            cell.volIdx[2],
            smallCCPer,
            aggdir,
            cell.volIdx[0],
            cell.volIdx[1],
            cell.volIdx[2]
            );
    return true;
}


WriteSplitTP::WriteSplitTP(double res,string fname,std::string basepath,const std::vector<Stereo_Pose_Data> &tasks,const typename CellDataT<Stereo_Pose_Data>::type &Vol):WriteTP(res,fname,basepath),_vol(Vol),_tasks(tasks){
    cmdfp =fopen(fname.c_str(),"w");
    if(!cmdfp){
        fprintf(stderr,"Can't create cmd file");
        exit(-1);
    }

}
bool WriteSplitTP::write_cmd(const picture_cell &cell){
    char shr_tmp[8192];
    sprintf(shr_tmp,"%s/treeBBClip ",
            _basepath.c_str());
    int v_count=0;

    foreach_vol(cur,_vol){
        if(cur->poses.size() == 0 || !cell.bboxMarginUnRot.intersects(cur->bounds.bbox))
            continue;
        sprintf(shr_tmp,"%s %s/clean_%d%d%d.ply",shr_tmp,aggdir,cur->volIdx[0],cur->volIdx[1],cur->volIdx[2]);
        v_count++;
    }
    if(v_count== 0)
        return false;

    sprintf(shr_tmp,"%s --bbox %.16f %.16f %.16f %.16f %.16f %.16f -gap -F --outfile %s/un-tmp-tex-clipped-diced-r_%04d_c_%04d.ply;",
            shr_tmp,
            cell.bbox.xMin(),
            cell.bbox.yMin(),
            cell.bbox.zMin(),
            cell.bbox.xMax(),
            cell.bbox.yMax(),
            cell.bbox.zMax(),
            diced_dir,
            cell.row,cell.col);
    fprintf(cmdfp,"%s %s/vcgapps/bin/sw-shadevis -P -n64 %s/un-tmp-tex-clipped-diced-r_%04d_c_%04d.ply ;",
            shr_tmp,_basepath.c_str(),diced_dir,
             cell.row,cell.col);
    fprintf(cmdfp,"%s/treeBBClip --bbox %.16f %.16f %.16f %.16f %.16f %.16f %s/vis-un-tmp-tex-clipped-diced-r_%04d_c_%04d.ply -dup -F --outfile %s/vis-tmp-tex-clipped-diced-r_%04d_c_%04d.ply \n",
            _basepath.c_str(),
           -FLT_MAX,
            -FLT_MAX,
            -FLT_MAX,
            FLT_MAX,
            FLT_MAX,
            FLT_MAX,
            diced_dir,
            cell.row,cell.col,
             diced_dir,cell.row,cell.col);
    char tp[1024];
    sprintf(tp,"%s/bbox-vis-tmp-tex-clipped-diced-r_%04d_c_%04d.ply.txt",diced_dir,cell.row,cell.col);
    FILE *bboxfp=fopen(tp,"w");

    for(int k=0; k < (int)cell.imagesMargin.size(); k++){
        const Stereo_Pose_Data *pose=(&_tasks[cell.imagesMargin[k]]);
        if(pose && pose->valid){
            fprintf(bboxfp, "%d %s " ,pose->id,pose->left_name.c_str());
            save_bbox_frame(pose->bbox,bboxfp);
            osg::Matrix texmat=osgTranspose(pose->mat);
            texmat=osg::Matrix::inverse(texmat);
            for(int f=0; f < 4; f++)
                for(int n=0; n < 4; n++)
                    fprintf(bboxfp," %lf",texmat(f,n));
            fprintf(bboxfp,"\n");
        }

    }
    fclose(bboxfp);

    return true;

}