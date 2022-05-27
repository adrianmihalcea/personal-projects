import { ComponentFixture, TestBed } from '@angular/core/testing';

import { HubItemsComponent } from './hub-items.component';

describe('HubItemsComponent', () => {
  let component: HubItemsComponent;
  let fixture: ComponentFixture<HubItemsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ HubItemsComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(HubItemsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
