#include <iostream>
#include "test/Image.h"
#include "map/map_attributes.h"
#include "highgui.h"
#include "cv.h"
#include "target_selector/closest_unexplored_target_selector.h"
#include <iostream>
#include "misc/pixelcoords.h"
#include "test/test_utilities.h"
#define MAP_SIZE 4096

#define d_PI_DOUBLE     		6.283185308

int main(){
	
	MapAttributes mapAtt(MAP_SIZE,MAP_SIZE);

	initMapFromImage("map1.png",&mapAtt);

	int prevxMax = mapAtt.prevxMax ;
	int prevyMax = mapAtt.prevyMax ;
	int prevxMin = mapAtt.prevxMin ;
	int prevyMin = mapAtt.prevyMin ;

	Coverage coverage(mapAtt.getHeight(), mapAtt.getWidth());
	//-----------------------------------
	makeDummyCoveragePatch(mapAtt.map,coverage.coverage,2046,2046,rand()%2000);
	for(unsigned int i=0;i<60;i++){
		int x=0,y=0;
		while(mapAtt.map[x][y]<130){
			x=prevxMin+rand()%(prevxMax-prevxMin);
			y=prevyMin+rand()%(prevyMax-prevyMin);
		}
		makeDummyCoveragePatch(mapAtt.map,coverage.coverage,x,y,rand()%300);
	}
	//--------------------------------
	ClosestUnexploredTargetSelector targetSelector(mapAtt, coverage);
	//~ VoronoiNodes voronoiNodes(&mapAtt, &coverage);
	//~ voronoiNodes.createVoronodes();

	CvScalar pixel;
	
	image mapSmall(prevxMax-prevxMin,prevyMax-prevyMin,"mapsmall",3);
	for(unsigned int i=0;i<prevxMax-prevxMin;i++)
		for(unsigned int j=0;j<prevyMax-prevyMin;j++){
			pixel.val[0]=pixel.val[1]=pixel.val[2]=mapAtt.map[i+prevxMin][j+prevyMin];
			if(coverage.coverage[i+prevxMin][j+prevyMin]>0)
				pixel.val[0]=255-coverage.coverage[i+prevxMin][j+prevyMin];
			mapSmall.setPoint(i,j,pixel);
			//~ if(voronoiNodes.voronoi.voronoi[i+prevxMin][j+prevyMin]){
				//~ pixel.val[0]=pixel.val[1]=0;
				//~ pixel.val[2]=255;
				//~ mapSmall.setPoint(i,j,pixel);
			//~ }
		}

	//robotPose
	pixel.val[0] = pixel.val[1]=255;
	pixel.val[2]=0;
	mapSmall.drawCross( mapAtt.robotPose.dx + 2048 - prevxMin, mapAtt.robotPose.dy + 2048 - prevyMin,5, pixel);
	
	
	PixelCoords target;
	
	
	targetSelector.selectTarget(&target);
	cout<<"in main"<<target.getXCoord()<<" "<<target.getYCoord()<<"\n";
	//target
	pixel.val[0] =255;
	pixel.val[1]=0;
	pixel.val[2]=0;
	//~ mapSmall.drawCross( target.getXCoord() + 2048 - prevxMin, target.getYCoord()+2048 - prevyMin,5, pixel);
	mapSmall.drawCross( target.getXCoord() - prevxMin, target.getYCoord() - prevyMin,5, pixel);
	
	
	
	pixel.val[0]=155;
	pixel.val[1]=155;
	pixel.val[2]=155;
	for (unsigned int i=0; i<targetSelector.coverageLimits.size(); i++){
		//~ mapSmall.setPoint(targetSelector.coverageLimits[i].getXCoord()+ 2048 - prevxMin, targetSelector.coverageLimits[i].getYCoord()+2048 - prevyMin,pixel);
		mapSmall.setPoint(targetSelector.coverageLimits[i].getXCoord() - prevxMin, targetSelector.coverageLimits[i].getYCoord() - prevyMin,pixel);
	}
	
	
	
	//~ drawGraph(mapSmall.display,voronoiNodes.nodes,prevxMin,prevxMax,prevyMin,prevyMax);
	mapSmall.saveImage("smallXartis.png");
	mapSmall.show(0);
}
	
	
	
