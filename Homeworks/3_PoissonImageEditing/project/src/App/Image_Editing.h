#pragma once

#include <QWidget>


namespace Image_Processing {
	class Image_Editing {
	public:
		Image_Editing();
		virtual ~Image_Editing();
		void set_image(QImage* image);
		void set_bg_image(QImage *bg_image);
		QImage* get_Edited_image();
		virtual void Edit() = 0;
	protected:
		QImage* m_image;
		QImage* m_bg_image;
		QImage* m_Edited_image;
	};
}