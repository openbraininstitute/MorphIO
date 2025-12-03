/* Copyright (c) 2013-2023, EPFL/Blue Brain Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <gsl/gsl-lite.hpp>
#include <morphio/version.h>

#include <filesystem>
#include <fstream>

#include "../src/shared_utils.hpp"

namespace fs = std::filesystem;

class TemporaryDirectoryFixture
{
  public:
    TemporaryDirectoryFixture(const std::string& subdir)
        : tmpDirectory(fs::temp_directory_path() / subdir) {
        fs::create_directories(tmpDirectory);
    }

    ~TemporaryDirectoryFixture() {
        fs::remove_all(tmpDirectory);
    }

    fs::path tmpDirectory;
};

class SymlinkFixture: TemporaryDirectoryFixture
{
  public:
    SymlinkFixture(const std::string& subdir)
        : TemporaryDirectoryFixture(subdir)
        , dirname(tmpDirectory / "dir")
        , symlinkDirname(tmpDirectory / "dir.symlink")
        , filename(tmpDirectory / "file")
        , symlinkFilename(tmpDirectory / "file.symlink")
        , doesntExist(tmpDirectory / "doesnt_exist")
        , symlinkDoesntExist(tmpDirectory / "doesnt_exist.symlink") {
        fs::create_directories(dirname);
        fs::create_directory_symlink(dirname, symlinkDirname);

        {
            std::ofstream f(filename);
            f << "foo";
        }
        fs::create_symlink(filename, symlinkFilename);
        fs::create_symlink(doesntExist, symlinkDoesntExist);
    }

    fs::path dirname;
    fs::path symlinkDirname;

    fs::path filename;
    fs::path symlinkFilename;

    fs::path doesntExist;
    fs::path symlinkDoesntExist;
};


TEST_CASE("morphio::is_regular_file", "[filesystem]") {
    auto fixture = SymlinkFixture("test_utilities.cpp");

    REQUIRE(!morphio::is_regular_file(fixture.dirname.string()));
    REQUIRE(!morphio::is_regular_file(fixture.symlinkDirname.string()));
    REQUIRE(morphio::is_regular_file(fixture.filename.string()));
    REQUIRE(morphio::is_regular_file(fixture.symlinkFilename.string()));
    REQUIRE(!morphio::is_directory(fixture.doesntExist.string()));
    REQUIRE(!morphio::is_directory(fixture.symlinkDoesntExist.string()));
}


TEST_CASE("morphio::is_directory", "[filesystem]") {
    auto fixture = SymlinkFixture("test_utilities.cpp");

    REQUIRE(morphio::is_directory(fixture.dirname.string()));
    REQUIRE(morphio::is_directory(fixture.symlinkDirname.string()));
    REQUIRE(!morphio::is_directory(fixture.filename.string()));
    REQUIRE(!morphio::is_directory(fixture.symlinkFilename.string()));
    REQUIRE(!morphio::is_directory(fixture.doesntExist.string()));
    REQUIRE(!morphio::is_directory(fixture.symlinkDoesntExist.string()));
}


TEST_CASE("morphio::join_path", "[filesystem]") {
    SECTION("join_path('', 'foo')", "[filesystem]") {
        std::string expected = "foo";
        std::string actual = morphio::join_path("", "foo");
        REQUIRE(expected == actual);
    }

    SECTION("join_path('bar', '/foo')", "[filesystem]") {
        std::string expected = "/foo";
        std::string actual = morphio::join_path("bar", "/foo");
        REQUIRE(expected == actual);
    }

    SECTION("join_path('bar', 'foo')", "[filesystem]") {
        std::string expected = "bar/foo";
        std::string actual = morphio::join_path("bar", "foo");
        REQUIRE(expected == actual);
    }
}

namespace {
using floatTypes = std::vector<morphio::floatType>;
}  // namespace


TEST_CASE("morphio::shared_utils") {
    using namespace morphio;
    SECTION("Errors") {
        const std::vector<floatType> diameters;
        const range<const floatType> d(diameters);

        const std::vector<Point> points;
        const range<const Point> p(points);

        CHECK_THROWS(_somaSurface(SOMA_SINGLE_POINT, d, p));
        CHECK_THROWS(_somaSurface(SOMA_NEUROMORPHO_THREE_POINT_CYLINDERS, d, p));
        CHECK_THROWS(_somaSurface(SOMA_SIMPLE_CONTOUR, d, p));  // not implemented
        CHECK_THROWS(_somaSurface(SOMA_UNDEFINED, d, p));
    }

    SECTION("SOMA_SINGLE_POINT") {
        const std::vector<Point> points = {{0, 0, 0}};
        const std::vector<floatType> diameters = {1};
        CHECK(centerOfGravity(points) == points[0]);
        CHECK_THAT(maxDistanceToCenterOfGravity(points), Catch::Matchers::WithinAbs(0.0, 0.001));
        CHECK_THAT(_somaSurface(SOMA_SINGLE_POINT, diameters, points), Catch::Matchers::WithinAbs(morphio::PI, 0.001));
        CHECK_THAT(_somaSurface(SOMA_SINGLE_POINT, diameters, points), Catch::Matchers::WithinAbs(morphio::PI, 0.001));
    }

    SECTION("SOMA_NEUROMORPHO_THREE_POINT_CYLINDERS") {
        const std::vector<Point> points = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
        const std::vector<floatType> diameters = {0.5, 1.5, 2.5};
        Point expected{0, 0, 0};
        CHECK(centerOfGravity(points) == expected);
        CHECK_THAT(maxDistanceToCenterOfGravity(points), Catch::Matchers::WithinAbs(std::sqrt(3.0), 0.001));
        CHECK_THAT(_somaSurface(SOMA_NEUROMORPHO_THREE_POINT_CYLINDERS, diameters, points),
              Catch::Matchers::WithinAbs(0.7854, 0.001));
    }

    SECTION("SOMA_CYLINDERS") {
        const std::vector<Point> points = {{0, 0, 0}, {1, 1, 1}, {2, 2, 2}};
        const std::vector<floatType> diameters = {0.5, 1.5, 2.5};
        Point expected{1, 1, 1};
        CHECK(centerOfGravity(points) == expected);
        CHECK_THAT(maxDistanceToCenterOfGravity(points), Catch::Matchers::WithinAbs(std::sqrt(3.0), 0.001));
        CHECK_THAT(_somaSurface(SOMA_CYLINDERS, diameters, points), Catch::Matchers::WithinAbs(16.99076, 0.001));
    }

    SECTION("SOMA_SIMPLE_CONTOUR") {
        const std::vector<Point> points = {{0, 0, 0}, {1, 1, 1}, {0, 1, 1}};
        const std::vector<floatType> diameters = {0.1, 0.1, 0.1};
        Point expected{1 / floatType{3}, 2 / floatType{3}, 2 / floatType{3}};
        CHECK(centerOfGravity(points) == expected);
        CHECK_THAT(maxDistanceToCenterOfGravity(points), Catch::Matchers::WithinAbs(1., 0.001));
        CHECK_THROWS(_somaSurface(SOMA_SIMPLE_CONTOUR, diameters, points));
    }
    /* CHECK(_somaSurface(SOMA_SINGLE_POINT, diameters, points) == Catch::Matchers::Approx(0.0)); */
}
