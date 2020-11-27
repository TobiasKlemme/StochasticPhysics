///---------------------------------------------------------------------------------------------------
// file:    MagneticProperties.h
//
// summary:     Declares the class for magnetic properties 
//
//            Copyright (c) 2016 Alexander Neumann.
//
// author: Alexander Neumann
// date: 05.06.2016

#ifndef INC_MagneticProperties_H
#define INC_MagneticProperties_H
///---------------------------------------------------------------------------------------------------

#pragma once

#include <type_traits>
#include <cstddef>
#include <exception>

#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <variant>


#include <Eigen/Core>

#include <MyCEL/basics/enumhelpers.h>
#include <MyCEL/basics/templatehelpers.h>

#include <MyCEL/math/Geometry.h>
#include <MyCEL/basics/BasicIncludes.h>

#include <SerAr/Core/NamedValue.h>

#include "Problems/Anisotropy/AnisotropyList.h"
#include "Selectors/AnisotropySelector.h"

#include "Properties/Anisotropy/All.hpp"

///-------------------------------------------------------------------------------------------------
/// <signature>    Properties </signature>
///
/// <summary>    Namesapce for different Property classes. </summary>
///-------------------------------------------------------------------------------------------------
namespace Properties
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary>    Class which defines the Magnetic Properties of a Particle. </summary>
    ///
    /// <seealso cref="T:IConfigFileAll{MagneticProperties{prec}}"/>
    /// <seealso cref="T:IMATLABFileWriteable{MagneticProperties{prec}}"/>
    ///-------------------------------------------------------------------------------------------------
    template<typename prec>
    class MagneticProperties
    {
        template<IAnisotropy value>
        struct anisotropy_enum_property_mapping { using type = typename Selectors::AnisotropyTypeSelector<value>::template input_parameter<prec>; };
        template <IAnisotropy... Values>
        using anisotropy_variant_helper_t = typename MyCEL::enum_variant_creator_t<IAnisotropy, anisotropy_enum_property_mapping, Values...>;
        template<IAnisotropy value>
        struct anisotropy_distribution_enum_property_mapping { using type = typename Selectors::AnisotropyTypeSelector<value>::template input_parameter<prec>::Distribution; };
        template <IAnisotropy... Values>
        using anisotropy_distribution_variant_helper_t = typename MyCEL::enum_variant_creator_t<IAnisotropy, anisotropy_distribution_enum_property_mapping, Values...>;
    public:
        using anisotropy_variant = typename MyCEL::apply_nttp_t<IAnisotropyValues,anisotropy_variant_helper_t>;
        using anisotropy_distribution_variant = typename MyCEL::apply_nttp_t<IAnisotropyValues,anisotropy_distribution_variant_helper_t>;
    private:
        typedef MagneticProperties<prec>        ThisClass;
        typedef Eigen::Matrix<prec, 3, 1>        Vec3D;

        prec                                    MagneticRadius{ 1E-9 };
        prec                                    SaturationMagnetisation{ 1.0 };
        prec                                    DampingConstant{ 1.0 };
        prec                                    GyromagneticRatio{ 1.0 };
        IAnisotropy                             TypeOfAnisotropy{ 0 };
    public:
        anisotropy_variant                      AnisotropyProperties{};

    protected:
    public:
        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Magnetic properties. </summary>
        ///
        /// <param name="MagRadius">         The Magnetic Radius. </param>
        /// <param name="SatMag">             The Saturation Magnetisation. </param>
        /// <param name="DampConst">         The Damping constant alpha. </param>
        /// <param name="GyromagRatio">      The Gyromagnetic Ratio. </param>
        /// <param name="TypeOfAniso">       The Type of Anisotropy. </param>
        /// <param name="AnisoConstants">    The Anisotropy Constants. </param>
        ///-------------------------------------------------------------------------------------------------
        constexpr inline MagneticProperties(prec MagRadius, prec SatMag, prec DampConst,
            prec GyromagRatio, IAnisotropy TypeOfAniso, anisotropy_variant Anisotropy)
            : MagneticRadius(MagRadius), SaturationMagnetisation(SatMag), DampingConstant(DampConst),
            GyromagneticRatio(GyromagRatio), TypeOfAnisotropy(TypeOfAniso), AnisotropyProperties(Anisotropy)
        {};
        constexpr inline MagneticProperties() = default;
        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets magnetic radius. </summary>
        ///
        /// <returns>    The magnetic radius. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline const prec& getMagneticRadius() const noexcept { return (MagneticRadius); };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Sets magnetic radius. </summary>
        ///
        /// <param name="value">    The new magnetic radius </param>
        ///-------------------------------------------------------------------------------------------------
        inline void setMagneticRadius(const prec &value) noexcept { MagneticRadius = value; };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Calculates the magnetic volume. </summary>
        ///
        /// <returns>    The magnetic volume. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline prec getMagneticVolume() const noexcept { return math::geometry::sphere::calcVolume(getMagneticRadius()); };
        
        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets saturation moment. </summary>
        ///
        /// <returns>    The saturation moment. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline  prec getSaturationMoment() const noexcept { return (getMagneticVolume()*SaturationMagnetisation); };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets saturation magnetisation. </summary>
        ///
        /// <returns>    The saturation magnetisation. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline const prec& getSaturationMagnetisation() const noexcept { return (SaturationMagnetisation); };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets the damping constant alpha. </summary>
        ///
        /// <returns>    The damping constant. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline const prec& getDampingConstant() const noexcept { return (DampingConstant); };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets the gyromagnetic ratio. </summary>
        ///
        /// <returns>    The gyromagnetic ratio. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline const prec& getGyromagneticRatio() const noexcept { return (GyromagneticRatio); };

        ///-------------------------------------------------------------------------------------------------
        /// <summary>    Gets the type of anisotropy. </summary>
        ///
        /// <returns>    The type of anisotropy. </returns>
        ///-------------------------------------------------------------------------------------------------
        inline const IAnisotropy& getTypeOfAnisotropy() const noexcept { return (TypeOfAnisotropy); };

        static inline std::string getSectionName() { return std::string{ "Magnetic_Properties" }; };

        template<IAnisotropy value>
        struct anisotropy_switch_case
        {
            template<typename Archive>
            void operator()(anisotropy_variant& aniso, Archive &ar)
            {
                using anisotropy_param_type = typename anisotropy_enum_property_mapping<value>::type;
                if(!std::holds_alternative<anisotropy_param_type>(aniso) )
                {
                    aniso = anisotropy_param_type{};
                }
                ar(Archives::createNamedValue(::Properties::Anisotropy::General<prec>::getSectionName(), std::get<anisotropy_param_type>(aniso)));
            }
        };

        struct anisotropy_default_switch_case
        {
            template<typename Archive>
            void operator()(anisotropy_variant& aniso, Archive &ar)
            {
                throw std::out_of_range{"Type of anisotropy unknown!"};
            }
        };


        template<IAnisotropy value>
        struct anisotropy_distribution_switch_case
        {
            template<typename Archive>
            void operator()(anisotropy_distribution_variant& anisodist, Archive &ar)
            {
                using distribution_param_type = typename anisotropy_distribution_enum_property_mapping<value>::type::Distribution;
                if(!std::holds_alternative<distribution_param_type>(anisodist) )
                {
                    aniso = distribution_param_type{};
                }
                ar(Archives::createNamedValue(::Properties::Anisotropy::Distribution<prec>::getSectionName(), std::get<distribution_param_type>(anisodist)));
            }
        };

        struct anisotropy_distribution_default_switch_case
        {
            template<typename Archive>
            void operator()(anisotropy_distribution_variant& anisodist, Archive &ar)
            {
                throw std::out_of_range{"Type of anisotropy distribution unknown!"};
            }
        };

        template<typename Archive>
        void serialize(Archive &ar)
        {
            ar(Archives::createNamedValue("Magnetic_radius", MagneticRadius));
            ar(Archives::createNamedValue("Saturation_magnetisation", SaturationMagnetisation));
            ar(Archives::createNamedValue("Damping_constant", DampingConstant));
            ar(Archives::createNamedValue("Gyromagnetic_ratio", GyromagneticRatio));

            std::string str{ to_string(TypeOfAnisotropy) };
            ar(Archives::createNamedValue(std::string{ "Type_of_anisotropy" }, str));
            TypeOfAnisotropy = from_string<decltype(TypeOfAnisotropy)>(str);

            MyCEL::enum_switch::run<decltype(TypeOfAnisotropy),anisotropy_default_switch_case,anisotropy_switch_case>(TypeOfAnisotropy,AnisotropyProperties,ar);
        }

        template<typename Archive>
        void serializeDistribution(anisotropy_distribution_variant &distvariant, Archive &ar) const
        {
            MyCEL::enum_switch::run<decltype(TypeOfAnisotropy), anisotropy_distribution_default_switch_case,anisotropy_switch_case>(TypeOfAnisotropy,distvariant,ar);
        }

        template<IAnisotropy value>
        decltype(auto) getAnisotropyProperties() const
        {
            using anisotropy_param_type = typename anisotropy_enum_property_mapping<value>::type;
            return std::get<anisotropy_param_type>(AnisotropyProperties);
        }
        template<IAnisotropy value>
        decltype(auto) getAnisotropyProperties()
        {
            using anisotropy_param_type = typename anisotropy_enum_property_mapping<value>::type;
            return std::get<anisotropy_param_type>(AnisotropyProperties);
        }

        ThisClass& operator+=(const ThisClass& rhs)                        // compound assignment (does not need to be a member,
        {                                                                // but often is, to modify the private members)
            if (TypeOfAnisotropy != rhs.getTypeOfAnisotropy())
            {
                throw std::runtime_error{ "Cannot add MagneticProperties due to different types of anisotropy!" };
            }
            MagneticRadius += rhs.getMagneticRadius();
            SaturationMagnetisation += rhs.getSaturationMagnetisation();
            DampingConstant += rhs.getDampingConstant();
            GyromagneticRatio += rhs.getGyromagneticRatio();
            std::visit([&rhs](auto&& arg) { (arg+=std::get<std::decay_t<decltype(arg)>>(rhs.AnisotropyProperties)); }, AnisotropyProperties);
            return *this;                                                // return the result by reference
        }

        // friends defined inside class body are inline and are hidden from non-ADL lookup
        friend ThisClass operator+(ThisClass lhs, const ThisClass& rhs) // passing lhs by value helps optimize chained a+b+c
        {                                                                // otherwise, both parameters may be const references
            lhs += rhs;                                                    // reuse compound assignment
            return lhs;                                                    // return the result by value (uses move constructor)
        }

        template<typename Number>
        std::enable_if_t<std::is_arithmetic<Number>::value, ThisClass&> operator/=(const Number& scalar)
        {
            MagneticRadius /= (double)scalar;
            SaturationMagnetisation /= (double)scalar;
            DampingConstant /= (double)scalar;
            GyromagneticRatio /= (double)scalar;
            std::visit([&scalar](auto&& arg) { (arg/=scalar); }, AnisotropyProperties);

            return *this;
        }

    };
}

#endif    // INC_MagneticProperties_H
// end of MagneticProperties.h
///---------------------------------------------------------------------------------------------------


