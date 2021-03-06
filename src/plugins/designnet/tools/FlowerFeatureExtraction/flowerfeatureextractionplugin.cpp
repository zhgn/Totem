#include "flowerfeatureextractionplugin.h"
#include "hscolorfeature.h"
#include "polorglcmblock.h"
#include "featureringcca.h"
#include <QtPlugin>
namespace FlowerFeatureExtraction{
FlowerFeatureExtractionPlugin::FlowerFeatureExtractionPlugin(QObject *parent)
{

}

FlowerFeatureExtractionPlugin::~FlowerFeatureExtractionPlugin()
{

}

void FlowerFeatureExtractionPlugin::extensionsInitialized()
{
	m_featureProcessor	= new HSColorFeature(0);
	m_polorGLCM			= new PolorGLCMBlock(0);
	m_featureRingCCA	= new FeatureRingCCA(0);
	addAutoReleasedObject(m_featureProcessor);
	addAutoReleasedObject(m_polorGLCM);
	addAutoReleasedObject(m_featureRingCCA);
}

bool FlowerFeatureExtractionPlugin::initialize( const QStringList &arguments, QString *errorString )
{
	return true;
}

}

Q_EXPORT_PLUGIN2(FlowerFeatureExtraction::FlowerFeatureExtractionPlugin, FlowerFeatureExtraction::FlowerFeatureExtractionPlugin)