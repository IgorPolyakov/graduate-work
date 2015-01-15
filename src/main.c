#include <QImage>
#include <QDebug>
#include <qmath.h>
#include <QTime>
#include <iostream>
//#include <omp.h>

int main(int argc, char *argv[])
{
    QImage firstImg,secondImg,outImg;
	if (argc < 3) {
		std::cout << "Please, input two images" << std::endl;
		return (-1);
	}
    if (!firstImg.load (QString (argv[1]))) {
		std::cout << "Cannot load first picture\n";
		return(-1);
	}
    if (!secondImg.load (QString (argv[2]))) {
		std::cout << "Cannot load second picture\n";
		return(-1);
	}
    if (!outImg.load (QString (argv[8]))) {
        std::cout << "Cannot load out picture\n";
		return(-1);
	}
    firstImg  =  firstImg.convertToFormat(QImage::Format_ARGB32);
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);

    for (ux = 0; ux < mainwidth; ++ux)
	{
		g = (lk_vec2f*) malloc(sizeof(lk_vec2f)*__LK_PYRAMIDE_LEVELS);
		d = (lk_vec2f*) malloc(sizeof(lk_vec2f)*__LK_PYRAMIDE_LEVELS);
		u = (lk_vec2f*) malloc(sizeof(lk_vec2f)*__LK_PYRAMIDE_LEVELS);
		iu= (lk_vec2i*) malloc(sizeof(lk_vec2f)*__LK_PYRAMIDE_LEVELS);
		b = (lk_vec2f*) malloc(sizeof(lk_vec2f)*__LK_ITERATIONS_COUNT);
		n = (lk_vec2f*) malloc(sizeof(lk_vec2f)*__LK_ITERATIONS_COUNT);

		for ( uy = 0; uy < mainheight; ++uy)
		{
			u0 = lk_vec2f(ux,uy);
			g[__LK_PYRAMIDE_LEVELS - 1] = lk_vec2f(0,0);
			for ( level = __LK_PYRAMIDE_LEVELS - 1; level >= 0; --level)
			{
                u[level] = u0 / powf (2,level);     //!< Положение точки u на текущем уровне
                iu[level] = u[level].round ();      //!< Округленное значение
				d[level] = lk_vec2f(0,0);
				imageGradXwindow = (const QRgb *) leftDX[level].bits ();
				imageGradYwindow = (const QRgb *) leftDY[level].bits ();
				Gradient_Matrix = lk_mat22f(0,0,0,0);
				for (wx = iu[level].x - __LK_WINDOW_HALF_SIZE_X; wx <= iu[level].x + __LK_WINDOW_HALF_SIZE_X; ++wx)
				{
					for (wy = iu[level].y - __LK_WINDOW_HALF_SIZE_Y; wy <= iu[level].y + __LK_WINDOW_HALF_SIZE_Y; ++wy)
					{
						width = leftDX[level].width ();
						invalid = !leftDY[level].valid (wx,wy);
						ix = invalid ? 0 : (qGray(imageGradXwindow[wx+wy*width]) - 127);
						iy = invalid ? 0 : (qGray(imageGradYwindow[wx+wy*width]) - 127);
						Gradient_Matrix.add (
						ix*ix,ix*iy,ix*iy,iy*iy
						);
					}//
				}//End of Gradient Matrix
				iterativeLK = lk_vec2f(0,0); //!< Результат смещения
				rightJL = (const QRgb *) rightP[level].bits ();
				leftJL = (const QRgb *) leftP[level].bits ();
				for (k = 0; k < __LK_ITERATIONS_COUNT; ++k)
				{
					b[k] = lk_vec2f(0,0);
					n[k] = lk_vec2f(0,0);
					for (wx = iu[level].x - __LK_WINDOW_HALF_SIZE_X ; wx <= iu[level].x + __LK_WINDOW_HALF_SIZE_X ; ++wx)
					{
						for (wy = iu[level].y - __LK_WINDOW_HALF_SIZE_Y ; wy <= iu[level].y + __LK_WINDOW_HALF_SIZE_Y; ++wy)
						{
							tx  = wx + g[level].x + iterativeLK.x;
							ty  = wy + g[level].y + iterativeLK.y;
							width = leftDX[level].width ();
							invalid = !leftDY[level].valid (wx,wy);
							invalidshift = !leftDY[level].valid (tx,ty);
							ix =  invalid ? 0 : (qGray(imageGradXwindow [wx + wy*width]) - 127);
							iy =  invalid ? 0 : (qGray(imageGradYwindow [wx + wy*width]) - 127);
							ik = ((invalid ? 0 : qGray(		 leftJL [wx + wy*width])) -
							(invalidshift  ? 0 : qGray(		rightJL [tx + ty*width])));
							b[k].add(ix*ik,iy*ik);
						}//
					}// End of image mismatch vector calc
					n[k] = Gradient_Matrix.solve (b[k]);
					iterativeLK = iterativeLK + n[k];
					if (n[k].x < 1 && n[k].y < 1) break;
				}// End of iterative LK
				d[level] = iterativeLK;
				if (level != 0 ) g[level-1] = (g[level] + d[level])*2;
			}// End of levels Loop
			res_disparity = (g[0]+d[0])*0.5;
			//res_disparity = lk_vec2f(0,0);
			int_disparity = res_disparity.round ();
            lValue = firstImg.pixel (ux,uy);
			rx = ux + int_disparity.x;
			ry = uy + int_disparity.y;
            if (outImg.valid(rx,ry))
			{
				rValue =
                outImg.pixel (rx,ry);
				diff = qRgb (
				qAbs(qRed  (rValue) - qRed  (lValue)),
				qAbs(qGreen(rValue) - qGreen(lValue)),
				qAbs(qBlue (rValue) - qBlue (lValue))
				);//rValue;
				warp_data[rx + mainwidth*ry] = lValue;
				diff_data[rx + mainwidth*ry] = diff;
			}
			magnitute = lk_utils::vec2angf (res_disparity);
			amplitude = lk_utils::vec2ampf (res_disparity);
			hsv_data[ux + mainwidth*uy] = lk_utils::hsv2rgb(magnitute,255,10*amplitude); //!< Картинка направлений смещения
			curp++;
			perc = float(curp)/all;
			if (perc > barrier)
			{
                difftime = timer1.elapsed () - curtime;
                curtime = timer1.elapsed();
                //std::cout <<  "" << perc*100 <<  "%\n";
                std::cout << "Progress " << perc*100 << "% \t\t"<< 0.001f * difftime << " (" << 0.001f * curtime << ")" << " seconds\n";
				barrier = perc + 0.1;
			}
		}//
		//	free(g);
		//	free(d);
		//	free(u);
		//	free(iu);
		//	free(b);
		//	free(n);
	}//End of each point
	std::cout << "Time for picture " << mainwidth << "x" << mainheight << " is " << 0.001f * timer1.elapsed() << " seconds\n";
    //QString nameHsvOutFile;
    //nameHsvOutFile = "HSV_MAP " + QString (argv[3]) + ".png";
    //QString nameDiffOutFile = "DIFF_MAP .png";

    QString nameHsvOutFile = ("HSV_MAP" + QString (argv[3]) + " " + QString (argv[4]) + " " + QString (argv[5])  + " " + QString (argv[6])  + " " + QString (argv[7])  + ".png");
    QString nameDiffOutFile = ("DIFF_MAP" + QString (argv[3]) + " " + QString (argv[4]) + " " + QString (argv[5])  + " " + QString (argv[6])  + " " + QString (argv[7])  + ".png");

    QImage((uchar*)hsv_data,mainwidth,mainheight,QImage::Format_ARGB32).save(nameHsvOutFile);
    QImage((uchar*)diff_data,mainwidth,mainheight,QImage::Format_ARGB32).save(nameDiffOutFile);
	QImage((uchar*)warp_data,mainwidth,mainheight,QImage::Format_ARGB32).save(argv[7]);

	delete [] leftP ;
	delete [] rightP;
	delete [] leftDX;
	delete [] leftDY;

	std::cout << "Done\n";
}//End of Main
