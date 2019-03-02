/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Fernando Iglesias, Thoralf Klein, Evgeniy Andreev, 
 *          Soeren Sonnenburg, Björn Esser
 */

#include <shogun/labels/StructuredLabels.h>

using namespace shogun;

CStructuredLabels::CStructuredLabels()
: CLabels()
{
	init();
}

CStructuredLabels::CStructuredLabels(int32_t num_labels)
: CLabels()
{
	init();
	m_labels = new CDynamicObjectArray(num_labels);
	SG_REF(m_labels);
}

CStructuredLabels::~CStructuredLabels()
{
	SG_UNREF(m_labels);
	if (m_score) {
        	delete[] m_score;
	}
	if (m_total_score) {
		SG_FREE(m_total_score);
	}
}

void CStructuredLabels::ensure_valid(const char* context)
{
	if ( m_labels == NULL )
		SG_ERROR("Non-valid StructuredLabels in %s", context)
}

CDynamicObjectArray* CStructuredLabels::get_labels() const
{
	SG_REF(m_labels);
	return m_labels;
}

CStructuredData* CStructuredLabels::get_label(int32_t idx)
{
	ensure_valid("CStructuredLabels::get_label(int32_t)");
	if ( idx < 0 || idx >= get_num_labels() )
		SG_ERROR("Index must be inside [0, num_labels-1]\n")

	return (CStructuredData*) m_labels->get_element(idx);
}

void CStructuredLabels::add_label(CStructuredData* label)
{
	ensure_valid_sdt(label);
	m_labels->push_back(label);
}

bool CStructuredLabels::set_label(int32_t idx, CStructuredData* label)
{
	ensure_valid_sdt(label);
	int32_t real_idx = m_subset_stack->subset_idx_conversion(idx);

	if ( real_idx < get_num_labels() )
	{
		return m_labels->set_element(label, real_idx);
	}
	else
	{
		return false;
	}
}

int32_t CStructuredLabels::get_num_labels() const
{
	if ( m_labels == NULL )
		return 0;
	else
		return m_labels->get_num_elements();
}

void CStructuredLabels::init()
{
	SG_ADD((CSGObject**) &m_labels, "m_labels", "The labels", MS_NOT_AVAILABLE);

	m_labels = NULL;
	m_sdt = SDT_UNKNOWN;

	m_score = NULL;
	m_total_score = NULL;
}

void CStructuredLabels::ensure_valid_sdt(CStructuredData* label)
{
	if ( m_sdt == SDT_UNKNOWN )
	{
		m_sdt = label->get_structured_data_type();
	}
	else
	{
		REQUIRE(label->get_structured_data_type() == m_sdt, "All the labels must "
				"belong to the same CStructuredData child class\n");
	}
}

void CStructuredLabels::init_score(int32_t num_labels)
{
	m_score = new SGVector <float64_t>[num_labels];
	m_total_score = SG_MALLOC(float64_t, num_labels);
}

void CStructuredLabels::set_score(int32_t j, SGVector <float64_t> score)
{
        m_score[j] = score;
}

SGVector<float64_t> CStructuredLabels::get_score(int32_t j)
{
        return m_score[j];
}

void CStructuredLabels::set_total_score(int32_t j, float64_t total_score)
{
        m_total_score[j] = total_score;
}

float64_t CStructuredLabels::get_total_score(int32_t j)
{
        return m_total_score[j];
}
