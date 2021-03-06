#include "bufferprocessor.h"
#include "data/matrixdata.h"
#include "property/doublerangeproperty.h"
#include "data/intdata.h"
#include "opencv2/core/core.hpp"
#include <vector>
using namespace std;
using namespace DesignNet;
class BufferProcessorPrivate
{
public: 
	BufferProcessorPrivate(BufferProcessor *processor)
		: m_processor(processor),
		m_bufferSizePort(new IntData(10, processor), Port::IN_PORT, "Buffer Size"),
		m_inputBufferPort(new MatrixData(processor), Port::IN_PORT, "Input Buffer"),
		m_outputMatrixPort(new MatrixData(processor), Port::OUT_PORT, "Output Matrix")
	{
		m_property = new DesignNet::DoubleRangeProperty("BufferSize", "BufferSize", processor);
		m_inputBufferPort.setMultiInputSupported(true);
		m_property->setDecimals(0);
		m_property->setRange(1, 10000);
	}
	DesignNet::DoubleRangeProperty *m_property; //!< �������Ĵ�С
	DesignNet::Port				m_bufferSizePort;
	DesignNet::Port				m_inputBufferPort;
	DesignNet::Port				m_outputMatrixPort;
	BufferProcessor*			m_processor;
	std::vector<cv::Mat>		m_mats;
};


BufferProcessor::BufferProcessor( DesignNet::DesignNetSpace *space, QObject* parent /*= 0*/ )
	: DesignNet::Processor(space, parent),
	d(new BufferProcessorPrivate(this))
{
	addProperty(d->m_property);
	addPort(&d->m_bufferSizePort);
	addPort(&d->m_inputBufferPort);
	addPort(&d->m_outputMatrixPort);
	d->m_property->setValue(10);
	setName(tr("Buffer"));
}

BufferProcessor::~BufferProcessor()
{
	delete d;
}

DesignNet::Processor* BufferProcessor::create( DesignNet::DesignNetSpace *space /*= 0*/ ) const
{
	return new BufferProcessor(space);
}

QString BufferProcessor::title() const
{
	return tr("Buffer");
}

QString BufferProcessor::category() const
{
	return tr("Operations");
}

bool BufferProcessor::process()
{
	cv::Mat mat;
	cv::merge(d->m_mats, mat);
	MatrixData data;
	data.setMatrix(mat);
	pushData(&data, "Output Matrix");
	d->m_mats.clear();
	return true;
}

void BufferProcessor::dataArrived( DesignNet::Port* port )
{
	if (port == &d->m_bufferSizePort)
	{
		d->m_property->setValue(((IntData*)d->m_bufferSizePort.data())->value());
	}
	else
	{
		MatrixData *mat = (MatrixData*)(port->data());
		if (d->m_mats.size() >= d->m_property->value())
		{
			d->m_mats.pop_back();
			d->m_mats.push_back(mat->getMatrix());
			setDataReady(true);
			return;
		}
		d->m_mats.push_back(mat->getMatrix());
	}
	
}

void BufferProcessor::propertyChanged( DesignNet::Property *prop )
{

}
