using Application;
using Application.Interfaces;
using FluentAssertions;

namespace Tests;

public class UnitTest1
{
    [Fact]
    public void MQTTService_WithNullAsRepository_ShouldThrowNullException()
    {
        // Arrange
        IMqttRepository repository = null;

        // Act
        Action action = () => new MqttService(repository);

        // Assert
        action.Should().Throw<ArgumentNullException>();
    }
}