using Application;
using Application.Interfaces;
using Domain;
using FluentAssertions;
using Moq;

namespace Tests;

public class MqttServiceTests
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
    
    [Fact]
    public void MQTTService_WithRepository_ShouldNotThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();

        // Act
        Action action = () => new MqttService(repository.Object);

        // Assert
        action.Should().NotThrow<ArgumentNullException>();
    }
    
    [Fact]
    public void Create_WithNullAsMqtt_ShouldThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = null;

        // Act
        Action action = () => service.create(mqtt);

        // Assert
        action.Should().Throw<NullReferenceException>();
    }
    
    [Fact]
    public void Create_WithMqtt_ShouldNotThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.create(It.IsAny<Mqtt>())).Returns(new Mqtt());
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();

        // Act
        Action action = () => service.create(mqtt);

        // Assert
        action.Should().NotThrow<NullReferenceException>();
    }
    
    [Fact]
    public void Create_WithMqtt_ShouldReturnMqtt()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.create(It.IsAny<Mqtt>())).Returns(new Mqtt());
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();

        // Act
        Mqtt returnMqtt = service.create(mqtt);

        // Assert
        returnMqtt.Should().BeOfType<Mqtt>();
    }
    
    [Fact]
    public void ReadAll_WithNullAsMqttList_ShouldThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.readAll()).Returns((List<Mqtt>) null);
        MqttService service = new MqttService(repository.Object);

        // Act
        Action action = () => service.readAll();

        // Assert
        action.Should().Throw<NullReferenceException>();
    }
}