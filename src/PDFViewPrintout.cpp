/////////////////////////////////////////////////////////////////////////////
// Name:        src/PDFViewPrintout.cpp
// Purpose:     wxPDFViewPrintout implementation
// Author:      Tobias Taschner
// Created:     2014-08-07
// Copyright:   (c) 2014 Tobias Taschner
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "private/PDFViewPrintout.h"
#include "private/PDFViewImpl.h"

wxPDFViewPrintout::wxPDFViewPrintout(wxPDFView* pdfView, bool forceBitmapPrint):
	wxPrintout(GetDocTitle(pdfView)),
	m_ctrl(pdfView),
	m_forceBitmapPrint(forceBitmapPrint)
{

}

wxString wxPDFViewPrintout::GetDocTitle(wxPDFView* pdfView)
{
	if (pdfView->GetDocumentTitle().empty())
		return _("PDF Document");
	else
		return pdfView->GetDocumentTitle();
}

bool wxPDFViewPrintout::OnPrintPage(int page)
{
	wxDC *dc = GetDC();
	if (dc && dc->IsOk())
	{
		if (HasPage(page))
			RenderPage(*dc, page - 1);
		return true;
	}
	else 
		return false;
}

bool wxPDFViewPrintout::HasPage(int page)
{
	return page > 0 && page <= m_ctrl->GetImpl()->GetPageCount();
}

void wxPDFViewPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
	*minPage = 1;
	*maxPage = m_ctrl->GetImpl()->GetPageCount();
	*selPageFrom = *minPage;
	*selPageTo = *maxPage;
}

bool wxPDFViewPrintout::OnBeginDocument(int startPage, int endPage)
{
	if (!wxPrintout::OnBeginDocument(startPage, endPage))
		return false;

	return true;
}

void wxPDFViewPrintout::OnPreparePrinting()
{
	wxPrintout::OnPreparePrinting();
}

void wxPDFViewPrintout::RenderPage(wxDC& dc, int pageIndex)
{
	int pageWidth, pageHeight;
	GetPageSizePixels(&pageWidth, &pageHeight);

	// Prepare DC
	dc.SetBackgroundMode(wxTRANSPARENT);

	// Draw page content
	wxRect printRect(0, 0, pageWidth, pageHeight);
	wxPDFViewPage& page = m_ctrl->GetImpl()->m_pages[pageIndex];
	page.DrawPrint(dc, printRect, m_forceBitmapPrint);
	page.Unload();
}
