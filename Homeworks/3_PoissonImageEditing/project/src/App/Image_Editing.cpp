#include "Image_Editing.h"
namespace Image_Processing {
	Image_Editing::Image_Editing()
	{
		m_image = nullptr;
		m_bg_image = nullptr;
		m_Edited_image = nullptr;
	}
	Image_Editing::~Image_Editing()
	{
		if (m_image != nullptr) {
			delete m_image;
		}
		if (m_bg_image != nullptr) {
			delete m_bg_image;
		}
		if (m_Edited_image != nullptr) {
			delete m_Edited_image;
		}
	}

	void Image_Editing::set_image(QImage* image)
	{
		m_image = new QImage;
		m_Edited_image = new QImage;
		*m_image = *image;
		*m_Edited_image = *image;
	}

	void Image_Editing::set_bg_image(QImage* bg_image)
	{
		m_bg_image = new QImage;
		*m_bg_image = *bg_image;
	}

	QImage* Image_Editing::get_Edited_image()
	{
		return m_Edited_image;
	}



}