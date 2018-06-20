#include "Tudat/Astrodynamics/OrbitDetermination/RotationalDynamicsPartials/constantTorquePartial.h"
#include "Tudat/SimulationSetup/EstimationSetup/createTorquePartials.h"

namespace tudat
{

namespace simulation_setup
{

std::shared_ptr< acceleration_partials::TorquePartial > createConstantTorqueRotationalDynamicsPartial(
        const std::pair< std::string, std::shared_ptr< simulation_setup::Body > > acceleratedBody,
        const basic_astrodynamics::SingleBodyTorqueModelMap& torqueVector )
{
    std::function< Eigen::Vector3d( ) > angularVelocityFunction =
            std::bind( &Body::getCurrentAngularVelocityVectorInLocalFrame, acceleratedBody.second );
    std::function< Eigen::Matrix3d( ) > inertiaTensorFunction =
            std::bind( &Body::getBodyInertiaTensor, acceleratedBody.second );

    std::function< double( ) > inertiaTensorNormalizationFunction;
    if( boost::dynamic_pointer_cast< gravitation::SphericalHarmonicsGravityField >(
                acceleratedBody.second->getGravityFieldModel( ) ) != NULL )
    {
        inertiaTensorNormalizationFunction =
                std::bind( &gravitation::SphericalHarmonicsGravityField::getInertiaTensorNormalizationFactor,
                             boost::dynamic_pointer_cast< gravitation::SphericalHarmonicsGravityField >(
                                 acceleratedBody.second->getGravityFieldModel( ) ) );
    }

    std::function< double( ) > gravitationalParameterFunction;
    if( acceleratedBody.second->getGravityFieldModel( ) != NULL )
    {
        gravitationalParameterFunction =
                std::bind( &gravitation::GravityFieldModel::getGravitationalParameter,
                             acceleratedBody.second->getGravityFieldModel( ) );
    }

    return std::make_shared< acceleration_partials::ConstantTorquePartial >(
                angularVelocityFunction, inertiaTensorFunction, inertiaTensorNormalizationFunction, gravitationalParameterFunction,
                torqueVector, acceleratedBody.first );
}

} // namespace simulation_setup

} // namespace tudat

