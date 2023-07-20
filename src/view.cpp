#include <iostream>
#include <cmath>//1226追加
#include <string>
#include "view.h"
#include "manager.h"
#include "network.h"
#include "stdlib.h"
//#include "mysmartphone.h"//1226追加
#include "common.h"
#include <cairomm/cairomm.h>

MyDrawingArea *drawingArea;//1226追加

MyDrawingArea::MyDrawingArea(BaseObjectType* o, const Glib::RefPtr<Gtk::Builder>& g):
Gtk::DrawingArea(o){
	scene=NULL;
	clearInput();
	input.up=0;
	input.down=0;
	input.left=0;
	input.right=0;
	//input.ax=0;//1226追加
	//input.ay=0;//1226追加
	//input.az=0;//1226追加
#ifdef USE_OPENGL//1226追加
	gl_config = gdk_gl_config_new_by_mode((GdkGLConfigMode)
			(GDK_GL_MODE_RGBA|GDK_GL_MODE_DEPTH|GDK_GL_MODE_DOUBLE));
	gtk_widget_set_gl_capability(&(o->widget), gl_config, NULL, TRUE,
			GDK_GL_RGBA_TYPE);
#endif
}

MyDrawingArea::~MyDrawingArea(void){
}

void MyDrawingArea::set_input(int x, int y){//パソコン画面に丸表示 //1226追加
	//input.x=x;
	//input.y=y;
}

/*void MyDrawingArea::set_angle(double ax, double ay, double az){//1226追加
	input.ax=ax;
	input.ay=ay;
	input.az=az;
}*/

void MyDrawingArea::on_realize(void){
	Gtk::DrawingArea::on_realize();
}
#if GTKMM3//1226追加
bool MyDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cc ){
	Gtk::DrawingArea::on_draw(cc);
#else
bool MyDrawingArea::on_expose_event( GdkEventExpose* e ){
	Cairo::RefPtr<Cairo::Context> cc = this->get_window()->create_cairo_context();
	Gtk::DrawingArea::on_expose_event(e);
#endif
	//std::cout << "Exposed" << std::endl;				//画面描写の度にexposeを表示

	if(scene==NULL)return true;

	//Cairo::RefPtr<Cairo::Context> cc = this->get_window()->create_cairo_context();


/*
#ifdef USE_OPENGL
	int z=ls/30;
	GdkGLContext *gl_context = gtk_widget_get_gl_context((GtkWidget *)this->gobj());
	GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable((GtkWidget *)this->gobj());

	gdk_gl_drawable_gl_begin(gl_drawable, gl_context);
	glViewport(0, 0, this->get_width(), this->get_height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, ls/10, 5.0*ls);
	gluLookAt(0, 3.0*ls, 2.0*ls, 0.0, 0.0, 0.0, 0.0, 9.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	GLfloat position[4]={-(GLfloat)ls, (GLfloat)ls, (GLfloat)ls, (GLfloat)1};
	GLfloat color[4];
	GLUquadricObj *q;

	glLightfv(GL_LIGHT0, GL_POSITION, position);
//	color[0]=0.0; color[1]=0.0; color[2]=0.0; color[3]=1.0;
//	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	color[0]=0.7; color[1]=0.7; color[2]=0.7; color[3]=1.0;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
//	color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=1.0;
//	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	q = gluNewQuadric();

	//	color[0]=0.2; color[1]=0.2; color[2]=0.2; color[3]=1.0;
	//	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	//	color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=1.0;
	//	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=0.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	color[0]=128.0;
	glMaterialfv(GL_FRONT, GL_SHININESS, color);
	color[0]=0; color[1]=0; color[2]=0; color[3]=1.0;
	glMaterialfv(GL_FRONT, GL_EMISSION, color);

	color[0]=0.5; color[1]=0.5; color[2]=0.5; color[3]=1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, -z/2, 0);
	glRotated(270, 1, 0, 0);
	gluDisk(q, 0, ls, 120, 10);
	glPopMatrix();

	for(int i=0; i<12; ++i){
		glPushMatrix();
		glRotated(-30*i+180, 0.0, 1.0, 0.0);
		glTranslated(0, -z/2, lm);
		gluCylinder(q, z, z, ls-lm, 10, 10);
		glPopMatrix();
	}

	color[0]=0.0; color[1]=0.0; color[2]=1.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, z, 0);
	glRotated(-6.0*scene->tm.tm_sec+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, ls, 10, 10);
	glPopMatrix();

	color[0]=0.0; color[1]=1.0; color[2]=0.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, 2*z, 0);
	glRotated(-6.0*(scene->tm.tm_min+scene->tm.tm_sec/60.0)+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, lm, 10, 10);
	glPopMatrix();

	color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glPushMatrix();
	glTranslated(0, 3*z, 0);
	glRotated(-30.0*(scene->tm.tm_hour+scene->tm.tm_min/60.0)+180, 0.0, 1.0, 0.0);
	gluCylinder(q, z/2, z/3, lh, 10, 10);
	glPopMatrix();

	for(int i=0; i<max_players; ++i){
		color[0]=((i+1)&1)>0; color[1]=((i+1)&2)>0; color[2]=((i+1)&4)>0; color[3]=1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		if(scene->p[i].attend){
			for(int j=0; j<max_dots; ++j){
				if(scene->p[i].dots[j].visible==1){
					glPushMatrix();
					glTranslated((scene->p[i].dots[j].x-this->get_width()/2)/2, 0, (scene->p[i].dots[j].y-this->get_height()/2)/2);
					glRotated(270, 1, 0, 0);
					gluDisk(q, z, 2*z, 120, 10);
					glPopMatrix();
				}
			}
		}
	}

	color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glCullFace(GL_FRONT);
	glPushMatrix();
	double roll=atan2(scene->p[0].ax, scene->p[0].az)*180/M_PI;
	double pitch=atan2(scene->p[0].ay, scene->p[0].az)*180/M_PI;
	glRotated(pitch, 1.0, 0.0, 0.0);
	glRotated(roll, 0.0, 0.0, 1.0);
	gdk_gl_draw_teapot(true, lh/4);
	glPopMatrix();

	if (gdk_gl_drawable_is_double_buffered(gl_drawable)){
		gdk_gl_drawable_swap_buffers(gl_drawable);
	}else{
		glFlush();
	}
	gdk_gl_drawable_gl_end(gl_drawable);
#else
*/
	Cairo::RefPtr<Cairo::ImageSurface> haikei;
	haikei = Cairo::ImageSurface::create_from_png("src/map.2.2.3.png");
/*
	//背景を描く(単色)
	cc->set_source_rgb(0,0.5,0.5);
	cc->rectangle(0,0,drawingArea->get_width(), drawingArea->get_height());
	cc->fill();
*/
	//背景を描く(写真)
	cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
				Cairo::FONT_WEIGHT_NORMAL);
	cc->save();

	//cc->rectangle(0,0,drawingArea->get_width(), drawingArea->get_height());
	//cc->translate((double) drawingArea->get_width(),
		//		(double) drawingArea->get_height());
	cc->rotate(0);
	cc->set_source(haikei, 0,0);
	cc->paint();
	//std::cout << "背景描写" << std::endl;
	cc->restore();

	//ここまで
	//↓弾(ユニット)を描いている場所！
	cc->set_line_width(4.0);
	cc->set_source_rgb(1.0, 0.0, 0.0);

	Cairo::RefPtr<Cairo::ImageSurface> pngg[max_players];
	pngg[0]= Cairo::ImageSurface::create_from_png("src/tank3.png");
	pngg[1]= Cairo::ImageSurface::create_from_png("src/tank4.png");
	pngg[2]= Cairo::ImageSurface::create_from_png("src/tank2.png");
	pngg[3]= Cairo::ImageSurface::create_from_png("src/tank1.png");

	Cairo::RefPtr<Cairo::ImageSurface> png[max_players];
	png[0] = Cairo::ImageSurface::create_from_png("src/castle2.png");
	png[1] = Cairo::ImageSurface::create_from_png("src/castle3.png");
	png[2] = Cairo::ImageSurface::create_from_png("src/castle4.png");
	png[3] = Cairo::ImageSurface::create_from_png("src/castle5.png");
/*
	Cairo::RefPtr<Cairo::ImageSurface> tree_width[4];
	tree_width[0] = Cairo::ImageSurface::create_from_png("src/field1.png");
	tree_width[1] = Cairo::ImageSurface::create_from_png("src/field1.png");
	tree_width[2] = Cairo::ImageSurface::create_from_png("src/field1.png");
	tree_width[3] = Cairo::ImageSurface::create_from_png("src/field1.png");

	Cairo::RefPtr<Cairo::ImageSurface> tree_height[4];
	tree_height[0] = Cairo::ImageSurface::create_from_png("src/field2.png");
	tree_height[1] = Cairo::ImageSurface::create_from_png("src/field2.png");
	tree_height[2] = Cairo::ImageSurface::create_from_png("src/field2.png");
	tree_height[3] = Cairo::ImageSurface::create_from_png("src/field2.png");

	Cairo::RefPtr<Cairo::ImageSurface> tree_center;
	tree_center = Cairo::ImageSurface::create_from_png("src/field3.png");
*/
	Cairo::RefPtr<Cairo::ImageSurface> capsule;
	capsule = Cairo::ImageSurface::create_from_png("src/recovery.png");

	Cairo::RefPtr<Cairo::ImageSurface> capsule1;
	capsule1 = Cairo::ImageSurface::create_from_png("src/bulletsize.png");
/*
	//障害物を描く(木?)

	for(int f(0); f < 4; f++){//横長い障害物
	cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
				Cairo::FONT_WEIGHT_NORMAL);
	cc->save();

	cc->translate((double) scene->Field[f].x,
				(double) scene->Field[f].y);
	cc->rotate(0);
	cc->set_source(tree_width[f], 0,0);
	cc->paint();
	//std::cout << "横長障害物" << std::endl;
	cc->restore();
	}

	for(int e(4); e < 8; e++){//縦長い障害物
	cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
				Cairo::FONT_WEIGHT_NORMAL);
	cc->save();

	cc->translate((double) scene->Field[e].x,
				(double) scene->Field[e].y);
	cc->rotate(0);
	cc->set_source(tree_height[e-4], 0,0);
	cc->paint();
	//std::cout << "縦長障害物" << std::endl;
	cc->restore();
	}

	cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,//中心の障害物
				Cairo::FONT_WEIGHT_NORMAL);
	cc->save();

	cc->translate((double) scene->Field[8].x,
				(double) scene->Field[8].y);
	cc->rotate(0);
	cc->set_source(tree_center, 0,0);
	cc->paint();
	//std::cout << "真ん中障害物" << std::endl;
	cc->restore();
*/

	//アイテムを描く(カプセル)
	for(int ite(0); ite<2; ite++){
		if(scene->Mono[ite].visible==1){
			cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
					Cairo::FONT_WEIGHT_NORMAL);
			cc->save();

			cc->translate((double) scene->Mono[ite].x,
					(double) scene->Mono[ite].y);
			cc->rotate(0);
			if(ite==0){
				cc->set_source(capsule,0,0);
				cc->paint();
		//		std::cout << "回復カプセル" << std::endl;
				cc->restore();
			}else if(ite==1){
				cc->set_source(capsule1,0,0);
				cc->paint();
		//		std::cout << "増弾カプセル" << std::endl;
				cc->restore();
			}
		}
	}


	for(int i=0; i<max_players; ++i){
		cc->set_source_rgb(((i+1)&1)>0, ((i+1)&2)>0, ((i+1)&4)>0);


		//城を描く
		if(scene->p[i].Castle.visible==1){
			cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
					Cairo::FONT_WEIGHT_NORMAL);
			cc->save();

			cc->translate((double) scene->p[i].Castle.x,
					(double) scene->p[i].Castle.y);
			cc->rotate(0);
			cc->set_source(png[i], -10,-10);
			cc->paint();
	//		std::cout << "お城★" << std::endl;
			cc->restore();
		}


		//if(scene->p[i].attend){								//参加していれば描く。
		for(int j=0; j<max_units; ++j){
			if(scene->p[i].units[j].visible==1){
				cc->set_line_width(4.0);
				cc->set_source_rgb(1-i, 0,i);
				if(i == 2){
					cc->set_source_rgb(0,1,0);
				}
				if(i == 3){
					cc->set_source_rgb(0.5,0.5,0.5);
				}
				//cc->arc(scene->p[i].units[j].x, scene->p[i].units[j].y, 5, 0, 2.0 * M_PI);
				//cc->stroke();
				cc->move_to(scene->p[i].units[j].x+6.0*cos(scene->p[i].units[j].rad),scene->p[i].units[j].y-6.0*sin(scene->p[i].units[j].rad));
				cc->line_to(scene->p[i].units[j].x+12.0*cos(scene->p[i].units[j].rad),scene->p[i].units[j].y-12.0*sin(scene->p[i].units[j].rad));
				cc->stroke();
				cc->set_font_size(10);
				cc->move_to(scene->p[i].units[j].x, scene->p[i].units[j].y-20);

				//unitの上にhpを表示
				std::stringstream text;
				text<< scene->p[i].units[j].hp;
				std::string text2 = text.str();
				cc->show_text(text2);
				cc->stroke();
				cc->move_to(scene->p[i].units[j].x-18, scene->p[i].units[j].y-20);
				cc->show_text("HP:");
				cc->stroke();

				//unitにlevelを表示
				std::stringstream text5;
				text5<< scene->p[i].units[j].level;
				std::string text6 = text5.str();
				cc->move_to(scene->p[i].units[j].x, scene->p[i].units[j].y-30);
				cc->show_text(text6);
				cc->stroke();
				cc->move_to(scene->p[i].units[j].x-18, scene->p[i].units[j].y-30);
				cc->show_text("Lv:");
				cc->stroke();
				//castleのhpを表示
				if(scene->p[i].Castle.visible==1){
					std::stringstream text3;
					text3<< scene->p[i].Castle.hp;
					std::string text4 = text3.str();
					cc->move_to(scene->p[i].Castle.x+4,scene->p[i].Castle.y-25);
					cc->show_text(text4);
					cc->stroke();
					cc->move_to(scene->p[i].Castle.x-14,scene->p[i].Castle.y-25);
					cc->show_text("HP:");
					cc->stroke();
				}

				//ユニットの画像描画
				if(scene->p[i].units[j].visible==1){

					cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
							Cairo::FONT_WEIGHT_NORMAL);
					cc->save();

					cc->translate((double) scene->p[i].units[j].x,
							(double) scene->p[i].units[j].y);
					cc->rotate(-(scene->p[i].units[j].rad));
					cc->set_source(pngg[i], -10,-10);
					cc->paint();
					cc->restore();
				}
/*
				//障害物を描く(四角)

				for(int k(0); k < 9; k++){
				cc->set_line_width(2.0);
				cc->move_to(scene->Field[k].x, scene->Field[k].y); cc->line_to(scene->Field[k].x, scene->Field[k].y_end);
				cc->move_to(scene->Field[k].x, scene->Field[k].y); cc->line_to(scene->Field[k].x_end, scene->Field[k].y);
				cc->move_to(scene->Field[k].x_end, scene->Field[k].y); cc->line_to(scene->Field[k].x_end, scene->Field[k].y_end);
				cc->move_to(scene->Field[k].x, scene->Field[k].y_end); cc->line_to(scene->Field[k].x_end, scene->Field[k].y_end);
				cc->stroke();
				}
*/
/*
				//障害物を描く(木?)

				for(int f(0); f < 4; f++){//横長い障害物
				cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
							Cairo::FONT_WEIGHT_NORMAL);
				cc->save();

				cc->translate((double) scene->Field[f].x,
							(double) scene->Field[f].y);
				cc->rotate(0);
				cc->set_source(tree_width[f], 0,0);
				cc->paint();
				cc->restore();
				}

				for(int e(4); e < 8; e++){//縦長い障害物
				cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
							Cairo::FONT_WEIGHT_NORMAL);
				cc->save();

				cc->translate((double) scene->Field[e].x,
							(double) scene->Field[e].y);
				cc->rotate(0);
			    cc->set_source(tree_height[e-4], 0,0);
				cc->paint();
				cc->restore();
				}

				cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,//中心の障害物
							Cairo::FONT_WEIGHT_NORMAL);
				cc->save();

				cc->translate((double) scene->Field[8].x,
							(double) scene->Field[8].y);
				cc->rotate(0);
				cc->set_source(tree_center, 0,0);
				cc->paint();
				cc->restore();
*/

/*
				//アイテムを描く(四角)
				for(int ite(0); ite<2; ite++){
					if(scene->Mono[ite].visible==1){
						cc->set_line_width(2.0);
						cc->set_source_rgb(0,0,1);
						cc->move_to(scene->Mono[ite].x, scene->Mono[ite].y); cc->line_to(scene->Mono[ite].x, scene->Mono[ite].y_end);
						cc->move_to(scene->Mono[ite].x, scene->Mono[ite].y); cc->line_to(scene->Mono[ite].x_end, scene->Mono[ite].y);
						cc->move_to(scene->Mono[ite].x_end, scene->Mono[ite].y); cc->line_to(scene->Mono[ite].x_end, scene->Mono[ite].y_end);
						cc->move_to(scene->Mono[ite].x, scene->Mono[ite].y_end); cc->line_to(scene->Mono[ite].x_end, scene->Mono[ite].y_end);
						cc->stroke();
					}
				}
*/
/*
				//アイテムを描く(カプセル)
				for(int ite(0); ite<2; ite++){
					if(scene->Mono[ite].visible==1){
						cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
								Cairo::FONT_WEIGHT_NORMAL);
						cc->save();

						cc->translate((double) scene->Mono[ite].x,
								(double) scene->Mono[ite].y);
						cc->rotate(0);
						if(ite==0){
							cc->set_source(capsule,0,0);
							cc->paint();
							cc->restore();
						}else if(ite==1){
							cc->set_source(capsule1,0,0);
							cc->paint();
							cc->restore();
						}
					}
				}
*/
/*
					//城を描く
				if(scene->p[i].Castle.visible==1){
					cc->select_font_face("Serif", Cairo::FONT_SLANT_NORMAL,
							Cairo::FONT_WEIGHT_NORMAL);
					cc->save();

					cc->translate((double) scene->p[i].Castle.x,
							(double) scene->p[i].Castle.y);
					cc->rotate(0);
					cc->set_source(png[i], -10,-10);
					cc->paint();
					std::cout << "お城★" << std::endl;
					cc->restore();
				}
*/

				for(int k=0; k<max_bullet; ++k){					//弾を描く所
					if(scene->p[i].units[j].shotbullet[k].visible==1){
						//std::cout << "プレイヤーID＝" <<i << std::endl;
						cc->set_line_width(2.0);
						cc->set_source_rgb(0.5-i/2, i/2,0);
						if(i == 2){
							cc->set_source_rgb(0,0,1/2);
						}
						if(i == 3){
							cc->set_source_rgb(0.25,0.25,0.25);
						}
						if(scene->p[i].units[j].shotbullet[k].kind==0){//通常
							cc->arc(scene->p[i].units[j].shotbullet[k].x, scene->p[i].units[j].shotbullet[k].y, 2, 0, 2.0 * M_PI);
							cc->stroke();
						}else if(scene->p[i].units[j].shotbullet[k].kind==1){//弾大きくなる
							cc->arc(scene->p[i].units[j].shotbullet[k].x, scene->p[i].units[j].shotbullet[k].y, 8, 0, 2.0 * M_PI);
							cc->stroke();
						}

					}
				}
			}
			//}
		}
		//<GAMEOVER>を表示する
		if(scene->p[i].gameover_flag==1){
			cc->set_source_rgb(0,0,0);
			cc->set_font_size(30);
			std::stringstream text5;
			text5<< "★G A M E O V E R★";
			std::string text6 = text5.str();

			if(i==0){
				cc->move_to(200,200);
			}else if(i==1){
				cc->move_to(1400,200);
			}else if(i==2){
				cc->move_to(200,800);
			}else{
				cc->move_to(-1400,-800);
			}

			cc->show_text(text6);
			cc->stroke();
		}
	}



//#endif								//↑ここまで。

	return true;
}

void MyDrawingArea::setScene(Scene *s){
	scene=s;
}

void MyDrawingArea::update(){
	this->queue_draw();
}

void MyDrawingArea::clearInput(void){
//	input.x=-1;
//	input.y=-1;
	input.key=0;
}

void MyDrawingArea::getInput(input_t *i){
//	*i=input;
	clearInput();
}

// PressイベントとReleaseイベントの両方を見ることで
// 押し続けている状態を把握できるようにしている
bool MyDrawingArea::on_key_press_event(GdkEventKey* k){		//ボタンが押されたときの挙動
	//	std::cout << "Pressed " << k->keyval << std::endl;
	switch(k->keyval){
	case GDK_KEY_Up:
		input.up=1;
		break;
	case GDK_KEY_Down:
		input.down=1;
		break;
	case GDK_KEY_Left:
		input.left=1;
		break;
	case GDK_KEY_Right:
		input.right=1;
		break;
	default:
		if(GDK_KEY_A<=k->keyval && k->keyval<=GDK_KEY_z){
			input.key=k->keyval;
		}
	}
	return true;
}

bool MyDrawingArea::on_key_release_event(GdkEventKey* k){			//ボタンを離したときの挙動
	//	std::cout << "Released " << k->keyval << std::endl;
	switch(k->keyval){
	case GDK_KEY_Up:
		input.up=0;
		break;
	case GDK_KEY_Down:
		input.down=0;
		break;
	case GDK_KEY_Left:
		input.left=0;
		break;
	case GDK_KEY_Right:
		input.right=0;
		break;
	default:
		if(GDK_KEY_A<=k->keyval && k->keyval<=GDK_KEY_z){
			input.key=0;
		}
	}
	return true;
}

bool MyDrawingArea::on_button_press_event (GdkEventButton* event){			//マウスのクリック情報
	//input.x=event->x;
	//input.y=event->y;
	return true;
}

MyImageMenuItem::MyImageMenuItem(BaseObjectType* o, const Glib::RefPtr<Gtk::Builder>& g):
			Gtk::ImageMenuItem(o){
	g->get_widget("window2", subWindow);
}

MyImageMenuItem::~MyImageMenuItem(void){
}

void MyImageMenuItem::on_activate(void){
	Gtk::ImageMenuItem::on_activate();

	Manager &mgr = Manager::get_instance();
	switch(id){
	case 0:
		if(mgr.get_state() != Manager::Run){
			mgr.set_state(Manager::Run);
			statusBar->push(Glib::ustring("Run"), statusId++);
			g_timeout_add(5000, eraseStatusbar, 0);
			switch(mgr.get_mode()){
			case Manager::Standalone:
				mgr.init_objects();
				mgr.attend_single_player();
				g_timeout_add(period, Manager::tick, NULL);
				break;
			case Manager::Server:
				mgr.init_objects();
				process_cmd(0, SCMD_START, 0, NULL);
				break;
			case Manager::Client:
				client_start();
				break;
			}
		}
		break;
	case 1:
		if(mgr.get_state() != Manager::Stop){
			mgr.set_state(Manager::Stop);
			statusBar->push(Glib::ustring("Stop"), statusId++);
			g_timeout_add(5000, eraseStatusbar, 0);
			switch(mgr.get_mode()){
			case Manager::Server:
				server_stop();
				break;
			case Manager::Client:
				client_stop();
				break;
			default:
				break;
			}
		}
		break;
	case 2:
		if(Manager::get_instance().get_state()==Manager::Stop){
			subWindow->show();
		}
		break;
	case 3://1226追加
		fileWindow->show();
		break;
	case 4://1226追加
		exit(0);
	/*case 3:
		exit(0);*/
	}
}
