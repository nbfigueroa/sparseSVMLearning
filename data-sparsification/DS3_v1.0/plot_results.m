%--------------------------------------------------------------------------
% Copyright @ Ehsan Elhamifar, 2014
%--------------------------------------------------------------------------

function plot_results(X,Y,Z,sInd)

warning off

sInd(sInd == size(Z,1)) = [];
oInd = find(Z(end,:) > 0.5);

figure
subplot(121)
plot(X(1,:),X(2,:),'o','LineWidth',4,'color','b','markersize',10,'markerfacecolor','b')
hold on
plot(X(1,sInd),X(2,sInd),'+','LineWidth',6,'color','r','markersize',16)
legend('source points','representatives','FontSize',24,'FontName','Times New Roman')
title('Source set','FontSize',24,'FontName','Times New Roman')
set(gca,'FontSize',24)
subplot(122)
plot(Y(1,:),Y(2,:),'o','LineWidth',4,'color','b','markersize',10,'markerfacecolor','b')
hold on
plot(Y(1,oInd),Y(2,oInd),'+','LineWidth',6,'color','r','markersize',16)
legend('target points','outliers','FontSize',24,'FontName','Times New Roman')
title('Target set','FontSize',24,'FontName','Times New Roman')
set(gca,'FontSize',24)


figure
imagesc(Z)
colormap(pink)
h = colormap;
colormap(flipud(h))
colorbar
set(gca,'FontSize',24)
set(gcf,'Renderer','Painters')
